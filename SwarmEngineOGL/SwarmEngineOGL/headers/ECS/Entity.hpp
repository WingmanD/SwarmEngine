#pragma once
#include <vector>

template <typename... ComponentTypes>
class Entity
{
public:
    Entity(int64_t id = -1) : _id(id) {}

    template <typename ComponentType>
    void Set(const ComponentType& value)
    {
        std::get<ComponentType>(_components) = value;
    }

    template <typename ComponentType>
    ComponentType& Get()
    {
        return std::get<ComponentType>(_components);
    }

    template <typename ComponentType>
    const ComponentType& Get() const
    {
        return std::get<ComponentType>(_components);
    }

    [[nodiscard]] int64_t GetId() const
    {
        return _id;
    }

    void SetId(int64_t id)
    {
        _id = id;
    }

private:
    int64_t _id = -1;

    std::tuple<ComponentTypes...> _components;
};
