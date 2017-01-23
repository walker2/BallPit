#ifndef FALCONLATINUM_MAINGAME_H
#define FALCONLATINUM_MAINGAME_H

#include <memory>
#include "SDL2/SDL.h"
#include "GL/glew.h"
#include "Falcon/States/StateManager.h"
#include "Falcon/2D/Sprite.h"
#include "Falcon/GLSL/ShaderProgram.h"
#include "Falcon/2D/GLTexture.h"
#include "Falcon/Window/Window.h"
#include "Falcon/2D/Camera/Camera2D.h"
#include "Falcon/Init/Falcon.h"
#include "Falcon/2D/SpriteBatch/SpriteBatch.h"
#include "Falcon/InputOutput/InputManager.h"
#include "Falcon/Timing/Timing.h"
#include "Falcon/Object/Projectile.h"
#include "Falcon/2D/SpriteFont/SpriteFont.h"
#include "Falcon/Audio/AudioEngine.h"
#include "Ball.h"
#include "Grid.h"
#include "BallRenderer.h"
#include "BallController.h"

enum class GameState { RUNNING, EXIT };

const int CELL_SIZE = 12;

class MainGame
{
public:
    ~MainGame();
    void run();

private:
    void initShaders();

private:
    void init();
    void initRenderers();
    void initBalls();
    void update(float deltaTime);
    void draw();
    void drawHud();
    void processInput();

    int m_screenWidth = 0;
    int m_screenHeight = 0;

    std::vector<Ball> m_balls; ///< All the balls
    std::unique_ptr<Grid> m_grid; ///< Grid for spatial partitioning for collision

    int m_currentRenderer = 0;
    std::vector<std::unique_ptr<BallRenderer> > m_ballRenderers;

    BallController m_ballController; ///< Controls balls

    Falcon::Window m_window; ///< The main window
    Falcon::SpriteBatch m_spriteBatch; ///< Renders all the balls
    std::unique_ptr<Falcon::SpriteFont> m_spriteFont; ///< For font rendering
    Falcon::Camera2D m_camera; ///< Renders the scene
    Falcon::InputManager m_inputManager; ///< Handles input
    Falcon::ShaderProgram m_textureProgram; ///< Shader for textures]

    Falcon::FPSLimiter m_fpsLimiter; ///< Limits and calculates fps
    float m_fps = 0.0f;

    GameState m_gameState = GameState::RUNNING; ///< The state of the game
};

#endif
