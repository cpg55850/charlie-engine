#pragma once
#include "Entity.hpp"
#include "System.hpp"
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

    // System management
    template <typename T, typename... TArgs>
    T& addSystem(TArgs&&... mArgs);

    template <typename T>
    T& getSystem();

    // Get all entities (for systems to query)
    std::vector<std::unique_ptr<Entity>>& getEntities();

private:
    std::vector<std::unique_ptr<Entity>> entities;
    std::array<std::vector<Entity*>, maxGroups> groupedEntities;
    std::vector<std::unique_ptr<System>> systems;
};

// Template implementations for System management
template <typename T, typename... TArgs>
T& Manager::addSystem(TArgs&&... mArgs) {
    auto system = std::make_unique<T>(std::forward<TArgs>(mArgs)...);
    T* rawPtr = system.get();
    systems.emplace_back(std::move(system));
    return *rawPtr;
}

template <typename T>
T& Manager::getSystem() {
    for (auto& system : systems) {
        if (T* ptr = dynamic_cast<T*>(system.get())) {
            return *ptr;
        }
    }
    throw std::runtime_error("System not found");
}