#pragma once
#include "Entity.hpp"
#include <array>
#include <vector>
#include <memory>
#include "Component.hpp" // Includes ComponentID, Group, etc.


class Manager {
public:
    void update(float deltaTime);
    void draw();
    void refresh();
    Entity& addEntity();
    void addToGroup(Entity* entity, Group group);
    std::vector<Entity*>& getGroup(Group group);

private:
    std::vector<std::unique_ptr<Entity>> entities;
    std::array<std::vector<Entity*>, maxGroups> groupedEntities;
};