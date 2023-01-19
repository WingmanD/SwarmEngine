#pragma once
#include "Widget.hpp"

class FramerateWidget : public Widget
{
public:
    FramerateWidget(std::string name, int* width, int* height);

    virtual void setupUI() override;

    void UpdateFramerate(double tickDuration);

private:
    double _tickDuration = 0.1;
};
