#include "ECS/EC/CubeObject.hpp"
#include "StaticMesh.hpp"
#include "StaticMeshComponent.hpp"

CubeObject::CubeObject(Engine* owner, StaticMesh* mesh, float mass, glm::vec3 pointGravityLocation,
                       float pointGravityMass) :
    Object(owner), _mass(mass), _pointGravityLocation(pointGravityLocation), _pointGravityMass(pointGravityMass)

{
    _mesh = new StaticMeshComponent(mesh);
    _mesh->attachTo(this);

    setScale(glm::vec3(glm::mix(0.01f, 0.5f,
                                glm::clamp((_mass - 100000.0f) / 500000.0f, 0.0f,
                                           1.0f))));
}

void CubeObject::tick(double deltaTime)
{
    Object::tick(deltaTime);

    const glm::vec3 radius = _pointGravityLocation - getLocation();
    const float distance = length(radius);
    constexpr float G = 6.67408e-11f;

    const float forceValue = G * _pointGravityMass * _mass /
        glm::pow(distance, 2.0f);
    const glm::vec3 force = forceValue * normalize(
        radius);

    const glm::vec3 acceleration = force / _mass;
    _velocity += acceleration * static_cast<float>(deltaTime);

    move(_velocity * static_cast<float>(deltaTime));
}
