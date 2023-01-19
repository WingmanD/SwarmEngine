#pragma once

#include <memory>

#include "Player.hpp"

class FramerateWidget;
class Camera;

class DefaultPlayer : public Player
{
public:
    DefaultPlayer(Engine* owningEngine);

    virtual void tick(double deltaTime) override;
    
    virtual void onMouseMove(double x, double y) override;
    virtual void onKey(int key, int scancode, int action, int mods) override;

private:
    std::unique_ptr<Camera> _camera = nullptr;
    
    double _speed = 2.5f;

    FramerateWidget* _framerateWidget = nullptr;
};
