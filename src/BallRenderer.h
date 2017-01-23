#ifndef FALCONLATINUM_BALLRENDERER_H
#define FALCONLATINUM_BALLRENDERER_H

#include "Falcon/GLSL/ShaderProgram.h"
#include "Falcon/2D/SpriteBatch/SpriteBatch.h"
#include "Ball.h"
#include <memory>

class BallRenderer
{
public:
    virtual void renderBalls(Falcon::SpriteBatch& spriteBatch, const std::vector<Ball>& balls,
                             const glm::mat4& projectionMatrix);
protected:
    std::unique_ptr<Falcon::ShaderProgram> m_program = nullptr;
};

// Visualizes kinetic energy
class MomentumBallRenderer : public BallRenderer
{
public:
    virtual void renderBalls(Falcon::SpriteBatch& spriteBatch, const std::vector<Ball>& balls,
                             const glm::mat4& projectionMatrix) override;
};

#endif
