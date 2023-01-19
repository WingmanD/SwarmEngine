#pragma once

#include "Transform.hpp"

struct ECSTransform
{
    glm::vec3 Location = glm::vec3(0.0f);
    glm::vec3 Rotation = glm::vec3(0.0f);
    glm::vec3 Scale = glm::vec3(1.0f, 1.0f, 1.0f);
};
