#pragma once

#include <memory>

#include "Object.hpp"

template <typename SystemType>
class ECSSystemWrapper : public Object
{
public:
    ECSSystemWrapper(Engine* engine, std::unique_ptr<SystemType> system)
        : Object(engine)
          , _system(std::move(system)) { }

    virtual void tick(double deltaTime) override
    {
        if (_system != nullptr)
        {
            _system->Tick(deltaTime);
        }
    }

    SystemType* GetSystem()
    {
        return _system.get();
    }

private:
    std::unique_ptr<SystemType> _system;
};
