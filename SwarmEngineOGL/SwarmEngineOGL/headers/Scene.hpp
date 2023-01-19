#pragma once

#include <chrono>
#include <vector>

#include "Camera.hpp"
#include "Light.hpp"
#include "Object.hpp"

/**
 *  The Scene class is used to store all the objects and lights in the scene.
 */
class Scene
{
    std::vector<Object*> objects;
    std::vector<Light*> lights;

    Camera* activeCamera = nullptr;

    double _lastTickDuration;
public:
    Scene() = default;

    void tick(double deltaTime);

    void addObject(Object* object);
    void addLight(Light* light);

    void removeObject(Object* object);
    void removeLight(Light* light);

    [[nodiscard]] Camera* getActiveCamera() const
    {
        return activeCamera;
    }

    void setActiveCamera(Camera* camera)
    {
        activeCamera = camera;
    }

    [[nodiscard]] std::vector<Object*> getObjects() const
    {
        return objects;
    }

    [[nodiscard]] std::vector<Light*> getLights() const
    {
        return lights;
    }

    [[nodiscard]] double getLastTickDuration() const
    {
        return _lastTickDuration;
    }

    ~Scene();
};
