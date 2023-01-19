#include "DefaultPlayer.hpp"

#include <memory>

#include "Camera.hpp"
#include "Engine.hpp"
#include "ECS/FramerateWidget.hpp"

DefaultPlayer::DefaultPlayer(Engine* owningEngine) : Player(owningEngine)
{
    _camera = std::make_unique<Camera>(Camera(engine->getWidthRef(), engine->getHeightRef(), {0.0f, 1.0f, 0.0f},
                                              {1.0f, 1.0f, 0.0f}));
    _camera->attachTo(this);
    engine->getScene()->setActiveCamera(_camera.get());

    _framerateWidget = new FramerateWidget("FPS", engine->getWidthRef(), engine->getHeightRef());
    _framerateWidget->setVisibility(true);
    engine->getWidgetManager()->addWidget(_framerateWidget);
}

void DefaultPlayer::tick(double deltaTime)
{
    Player::tick(deltaTime);

    _framerateWidget->UpdateFramerate(engine->getLastTickDuration());

    float speed = _speed * deltaTime;
    if (glfwGetKey(engine->getOpenGLWindow(), GLFW_KEY_LEFT_SHIFT)) speed *= 4.0f;

    if (glfwGetKey(engine->getOpenGLWindow(), GLFW_KEY_W)) move(_camera->getFront() * speed);
    if (glfwGetKey(engine->getOpenGLWindow(), GLFW_KEY_S)) move(-_camera->getFront() * speed);
    if (glfwGetKey(engine->getOpenGLWindow(), GLFW_KEY_A)) move(-_camera->getRight() * speed);
    if (glfwGetKey(engine->getOpenGLWindow(), GLFW_KEY_D)) move(_camera->getRight() * speed);
    if (glfwGetKey(engine->getOpenGLWindow(), GLFW_KEY_Q)) move(_camera->getViewUp() * speed);
    if (glfwGetKey(engine->getOpenGLWindow(), GLFW_KEY_E)) move(-_camera->getViewUp() * speed);
}

void DefaultPlayer::onMouseMove(double x, double y)
{
    Player::onMouseMove(x, y);

    static int lastX = -1;
    static int lastY = -1;

    if (lastX == -1 && lastY == -1)
    {
        lastX = x;
        lastY = y;
    }

    float xOffset = x - lastX;
    float yOffset = lastY - y;

    lastX = x;
    lastY = y;

    constexpr float sensitivity = 0.005f;
    xOffset *= sensitivity;
    yOffset *= sensitivity;

    _camera->rotate(glm::vec3(0, yOffset, xOffset));
}

void DefaultPlayer::onKey(int key, int scancode, int action, int mods)
{
    Player::onKey(key, scancode, action, mods);
    
}
