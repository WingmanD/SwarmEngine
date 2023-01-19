#pragma once
#include <functional>

#include "Node.hpp"
#include "World.hpp"


template <typename EntityType>
class System
{
public:
    explicit System(std::shared_ptr<World> world) : _world(world) {}

    void Tick(float deltaTime)
    {
        if (_world == nullptr)
        {
            return;
        }

        Node<EntityType>* node = _world->Query<EntityType>();

        if (node == nullptr)
        {
            return;
        }

        // todo use ForEach 
        for (auto& entity : node->Entities)
        {
            for (const auto& function : _functions)
            {
                function(deltaTime, entity);
            }
        }
    }

    void RegisterFunction(const std::function<void(float deltaTime, EntityType& entity)>& function)
    {
        _functions.emplace_back(function);
    }

    [[nodiscard]] World* GetWorld() const
    {
        return _world.get();
    }
    
private:
    std::shared_ptr<World> _world = nullptr;

    std::vector<std::function<void(float deltaTime, EntityType& entity)>> _functions;
};
