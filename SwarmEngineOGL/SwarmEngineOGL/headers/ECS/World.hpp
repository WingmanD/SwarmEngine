#pragma once

#include <vector>
#include <any>

class World {

public:
    World() = default;

    World(const World &other) = delete;

    World(World &&other) = delete;

    ~World() {
        // todo smart ptr instead
    }

    template<typename EntityType>
    Node<EntityType> *Query() {
        for (auto &node: _nodes) {
            if (auto desiredNode = std::any_cast<Node<EntityType> *>(node)) {
                return desiredNode;
            }
        }

        return nullptr;
    }

    template<typename EntityType>
    void RegisterNode(Node<EntityType> *node) {
        _nodes.push_back(node);
    }

    template<typename EntityType>
    Node<EntityType> *AddEntity(EntityType &entity) {
        entity.SetId(nextEntityId);

        auto *node = Query<EntityType>();
        if (node == nullptr) {
            auto *newNode = new Node<EntityType>();
            newNode->Entities.emplace_back(entity);

            RegisterNode(newNode);
            return newNode;
        }
        
        node->Entities.emplace_back(entity);
        return node;
    }

    template<typename EntityType>
    Node<EntityType> *CreateEntities(int64_t count) {
        auto *node = Query<EntityType>();
        if (node == nullptr) {
            node = new Node<EntityType>();
            RegisterNode(node);
        }

        for (int64_t i = 0; i < count; ++i) {
            node->Entities.emplace_back(EntityType(nextEntityId++));
        }

        return node;
    }

private:
    std::vector<std::any> _nodes;

    int64_t nextEntityId = 0;
};
