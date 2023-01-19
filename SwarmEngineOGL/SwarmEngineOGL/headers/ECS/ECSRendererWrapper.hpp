#pragma once

#include "Object.hpp"
#include <memory>

template <typename SystemType>
class ECSRendererWrapper : public Object
{
public:
    ECSRendererWrapper(Engine* engine, std::unique_ptr<SystemType> system)
        : Object(engine)
          , _system(std::move(system)) { }

    virtual void draw() override
    {
        if (_system != nullptr)
        {
            _system->Tick(0.0f);
        }
    }

    SystemType* GetSystem()
    {
        return _system.get();
    }

private:
    std::unique_ptr<SystemType> _system;
};
