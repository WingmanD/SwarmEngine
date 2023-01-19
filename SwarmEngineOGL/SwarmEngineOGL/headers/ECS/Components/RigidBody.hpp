#pragma once
#include <glm/vec3.hpp>

struct RigidBody
{
    float Mass = 1.0f;
    glm::vec3 Force = glm::vec3(0);
    glm::vec3 Acceleration = glm::vec3(0);
    glm::vec3 Velocity = glm::vec3(0);
};
