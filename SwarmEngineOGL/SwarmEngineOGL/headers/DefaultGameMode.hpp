#pragma once
#include "GameMode.hpp"

class DefaultGameMode : public GameMode
{
public:
    DefaultGameMode(Engine* owner);
    
    virtual void start() override;
    virtual void end() override;
    virtual void reset() override;
};
