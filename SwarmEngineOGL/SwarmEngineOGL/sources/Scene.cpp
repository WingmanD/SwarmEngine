#include "Scene.hpp"

#include <chrono>

void Scene::tick(double deltaTime)
{
    auto start = std::chrono::high_resolution_clock::now();
    for (const auto& object : objects) { object->tick(deltaTime); }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    
    _lastTickDuration = elapsed.count();
}

void Scene::addObject(Object* object) { objects.emplace_back(object); }
void Scene::addLight(Light* light) { lights.emplace_back(light); }

void Scene::removeObject(Object* object) { erase(objects, object); }

void Scene::removeLight(Light* light) { erase(lights, light); }

Scene::~Scene() {
    for (const auto object : objects)
        delete object;
}
