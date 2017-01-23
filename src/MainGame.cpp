#include <random>
#include "MainGame.h"
#include "Falcon/ResourceManager/ResourceManager.h"

// Helpful constants.
const float DESIRED_FPS = 60.0f; // FPS the game is designed to run at
const int MAX_PHYSICS_STEPS = 6; // Max number of physics steps per frame
const float MS_PER_SECOND = 1000; // Number of milliseconds in a second
const float DESIRED_FRAMETIME = MS_PER_SECOND / DESIRED_FPS; // The desired frame time per frame
const float MAX_DELTA_TIME = 1.0f; // Maximum size of deltaTime


MainGame::~MainGame() {
    // Empty
}

void MainGame::run() {
    init();
    initBalls();

    // Start our previousTicks variable
    Uint32 previousTicks = SDL_GetTicks();

    // Game loop
    while (m_gameState == GameState::RUNNING)
    {
        m_fpsLimiter.begin();
        processInput();

        // Calculate the frameTime in milliseconds
        Uint32 newTicks = SDL_GetTicks();
        Uint32 frameTime = newTicks - previousTicks;
        previousTicks = newTicks; // Store newTicks in previousTicks so we can use it next frame

        // Get the total delta time
        float totalDeltaTime = (float)frameTime / DESIRED_FRAMETIME;

        int i = 0; // This counter makes sure we don't spiral to death!

        // Loop while we still have steps to process.
        while (totalDeltaTime > 0.0f && i < MAX_PHYSICS_STEPS)
        {
            // The deltaTime should be the the smaller of the totalDeltaTime and MAX_DELTA_TIME
            float deltaTime = std::min(totalDeltaTime, MAX_DELTA_TIME);
            // Update all physics here and pass in deltaTime

            update(deltaTime);

            // Since we just took a step that is length deltaTime, subtract from totalDeltaTime
            totalDeltaTime -= deltaTime;
            // Increment our frame counter so we can limit steps to MAX_PHYSICS_STEPS
            i++;
        }

        m_camera.update();
        draw();
        m_fps = m_fpsLimiter.end();
    }
}
void MainGame::initShaders()
{
    m_textureProgram.compileShaders("include/Falcon/Shaders/textureShading.vert", "include/Falcon/Shaders/textureShading.frag");
    m_textureProgram.addAttribute("vertexPosition");
    m_textureProgram.addAttribute("vertexColor");
    m_textureProgram.addAttribute("vertexUV");
    m_textureProgram.linkShaders();
}

void MainGame::init()
{
    Falcon::init();

    m_screenWidth = 1280;
    m_screenHeight = 720;

    m_window.create("EC0H", m_screenWidth, m_screenHeight, 0);
    initShaders();
    glewExperimental = GL_TRUE;
    glewInit();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    m_camera.init(m_screenWidth, m_screenHeight);
    // Point the camera to the center of the screen
    m_camera.setPosition(glm::vec2(m_screenWidth / 2.0f, m_screenHeight / 2.0f));

    m_spriteBatch.init();
    // Initialize sprite font
    m_spriteFont = std::make_unique<Falcon::SpriteFont>("media/Fonts/pixel_font.ttf", 40);

    m_fpsLimiter.init(999.0f);

    initRenderers();

}

void MainGame::initRenderers()
{
    m_ballRenderers.push_back(std::make_unique<BallRenderer>());
    m_ballRenderers.push_back(std::make_unique<MomentumBallRenderer>());
}

struct BallSpawn
{
    BallSpawn(const Falcon::Color& colr,
              float rad, float m, float minSpeed,
              float maxSpeed, float prob) :
            color(colr),
            radius(rad),
            mass(m),
            randSpeed(minSpeed, maxSpeed),
            probability(prob)
    {
        // Empty
    }
    Falcon::Color color;
    float radius;
    float mass;
    float probability;
    std::uniform_real_distribution<float> randSpeed;
};

#include <iostream>
void MainGame::initBalls()
{

    // Initialize the grid
    m_grid = std::make_unique<Grid>(m_screenWidth, m_screenHeight, CELL_SIZE);

    // Number of balls to spawn
    const int NUM_BALLS = 10000;

    m_balls.reserve(NUM_BALLS * 2);
}

void MainGame::update(float deltaTime)
{
    m_ballController.updateBalls(m_balls, m_grid.get(), deltaTime, m_screenWidth, m_screenHeight);
}

void MainGame::draw()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    // Set the base depth to 1.0
    glClearDepth(1.0);
    // Clear the color and depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);

    // Grab the camera matrix
    glm::mat4 projectionMatrix = m_camera.getCameraMatrix();

    m_ballRenderers[m_currentRenderer]->renderBalls(m_spriteBatch, m_balls, projectionMatrix);

    m_textureProgram.use();

    // Make sure the shader uses texture 0
    GLint textureUniform = m_textureProgram.getUniformLocation("mySampler");
    glUniform1i(textureUniform, 0);

    GLint pUniform = m_textureProgram.getUniformLocation("P");
    glUniformMatrix4fv(pUniform, 1, GL_FALSE, &projectionMatrix[0][0]);

    drawHud();

    m_textureProgram.unuse();

    m_window.swapBuffer();
}

void MainGame::drawHud()
{
    const Falcon::Color fontColor(255, 0, 0, 255);
    // Convert float to char *
    char buffer[64];
    sprintf(buffer, "%.1f", m_fps);

    m_spriteBatch.begin();
    m_spriteFont->draw(m_spriteBatch, buffer, glm::vec2(0.0f, m_screenHeight - 32.0f),
                       glm::vec2(1.0f), 0.0f, fontColor);
    m_spriteBatch.end();
    m_spriteBatch.renderBatch();
}

void MainGame::processInput()
{
    m_inputManager.update();

    SDL_Event evnt;

    while (SDL_PollEvent(&evnt))
    {
        switch (evnt.type)
        {
            case SDL_QUIT:
                m_gameState = GameState::EXIT;
                break;
            case SDL_MOUSEMOTION:
                m_ballController.onMouseMove(m_balls, (float)evnt.motion.x, (float)m_screenHeight - (float)evnt.motion.y);
                m_inputManager.setMouseCoords((float)evnt.motion.x, (float)evnt.motion.y);
                break;
            case SDL_KEYDOWN:
                m_inputManager.pressKey(evnt.key.keysym.sym);
                break;
            case SDL_KEYUP:
                m_inputManager.releaseKey(evnt.key.keysym.sym);
                break;
            case SDL_MOUSEBUTTONDOWN:
                m_ballController.onMouseDown(m_balls, (float)evnt.button.x, (float)m_screenHeight - (float)evnt.button.y);
                m_inputManager.pressKey(evnt.button.button);
                break;
            case SDL_MOUSEBUTTONUP:
                m_ballController.onMouseUp(m_balls);
                m_inputManager.releaseKey(evnt.button.button);
                break;
        }
    }

    if (m_inputManager.isKeyPressed(SDLK_ESCAPE))
    {
        m_gameState = GameState::EXIT;
    }
    if (m_inputManager.isKeyDown(SDL_BUTTON_LEFT))
    {
        m_balls.emplace_back(6, 10, glm::vec2(m_inputManager.getMouseCoords().x, m_screenHeight - m_inputManager.getMouseCoords().y), glm::vec2(0.0f, 0.0f),
                             Falcon::ResourceManager::getTexture("media/Textures/circle.png").id, Falcon::Color(255, 0, 0, 255));
        m_grid->addBall(&m_balls.back());
    }
    // Handle gravity changes
    if (m_inputManager.isKeyPressed(SDLK_LEFT))
    {
        m_ballController.setGravityDirection(GravityDirection::LEFT);
    }
    else if (m_inputManager.isKeyPressed(SDLK_RIGHT))
    {
        m_ballController.setGravityDirection(GravityDirection::RIGHT);
    }
    else if (m_inputManager.isKeyPressed(SDLK_UP))
    {
        m_ballController.setGravityDirection(GravityDirection::UP);
    }
    else if (m_inputManager.isKeyPressed(SDLK_DOWN))
    {
        m_ballController.setGravityDirection(GravityDirection::DOWN);
    }
    else if (m_inputManager.isKeyPressed(SDLK_SPACE))
    {
        m_ballController.setGravityDirection(GravityDirection::NONE);
    }

    // Switch renderers
    if (m_inputManager.isKeyPressed(SDLK_1))
    {
        m_currentRenderer++;
        if (m_currentRenderer >= (int)m_ballRenderers.size())
        {
            m_currentRenderer = 0;
        }
    }
}