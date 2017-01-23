
#ifndef FALCONLATINUM_BALL_H
#define FALCONLATINUM_BALL_H

#include <glm/glm.hpp>
#include "Falcon/2D/Vertex.h"

struct Cell;

struct Ball
{
    Ball(float radius, float mass, const glm::vec2 &pos,
         const glm::vec2 &vel, unsigned int textureId,
         const Falcon::Color &color);

    float radius;
    float mass;
    glm::vec2 velocity;
    glm::vec2 position;
    unsigned int textureId = 0;
    Falcon::Color color;
    Cell *ownerCell = nullptr;
    int cellVectorIndex = -1;
};

#endif //FALCONLATINUM_BALL_H
