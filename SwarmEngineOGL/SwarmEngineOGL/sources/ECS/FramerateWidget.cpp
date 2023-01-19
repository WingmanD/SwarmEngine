#include "ECS/FramerateWidget.hpp"

FramerateWidget::FramerateWidget(std::string name, int* width, int* height) : Widget(name, width, height)
{
    windowFlags = ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBringToFrontOnFocus;
}

void FramerateWidget::setupUI()
{
    ImGui::Text("Tickrate: %d, CPU time: %f", static_cast<int>(1.0 / _tickDuration), _tickDuration);
}

void FramerateWidget::UpdateFramerate(double tickDuration)
{
    _tickDuration = tickDuration;
}
