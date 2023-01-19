#pragma once
#include "Object.hpp"

class StaticMesh;
class StaticMeshComponent;

class CubeObject : public Object
{
public:
    CubeObject(Engine* owner, StaticMesh* mesh, float mass, glm::vec3 pointGravityLocation, float pointGravityMass);

    void tick(double deltaTime) override;

private:
    StaticMeshComponent* _mesh;
    
    float _mass;
    
    glm::vec3 _pointGravityLocation;
    float _pointGravityMass;

    glm::vec3 _velocity = glm::vec3(0.0f);
};
