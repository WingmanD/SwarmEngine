#pragma once
#include <functional>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

struct NodeBase
{
    NodeBase* Parent = nullptr;
    std::string Type;
    std::map<std::string, NodeBase*> Children;

    NodeBase() = default;

    NodeBase(const NodeBase& other) = default;

    NodeBase(NodeBase&& other) = default;

    ~NodeBase() = default;

    template <typename EntityType>
    void AddEntity(EntityType* entity)
    {
        throw std::logic_error("Function not yet implemented");
    }
};

template <typename EntityType>
struct Node : NodeBase
{
    std::vector<EntityType> Entities;

    void ForEach(const std::function<void(EntityType& entity)>& function)
    {
        for (auto& entity : Entities)
        {
            function(entity);
        }
    }
};
