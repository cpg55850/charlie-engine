#pragma once
#include "Entity.hpp"
#include "System.hpp"
#include <array>
#include <vector>
#include <memory>
#include "Component.hpp"
#include "ComponentManager.hpp"
#include <unordered_map>
#include <stdexcept>


class Manager {
public:
    void update(float deltaTime);
    void refreshGroups();      // Only prunes invalid group members
    void destroyDeadEntities(); // Handles entity destruction + component cleanup
    void refresh();            // Backwards compatibility: calls both
    Entity& addEntity();
    void addToGroup(Entity* entity, Group group);
    void removeFromAllGroups(Entity* entity); // Safely purge entity pointers from all groups
    std::vector<Entity*>& getGroup(Group group);

    // System management
    template <typename T, typename... TArgs>
    T& addSystem(TArgs&&... mArgs);

    template <typename T>
    T& getSystem();

    template <typename T>
    bool hasSystem() {
        for (auto& system : systems) {
            if (dynamic_cast<T*>(system.get())) return true;
        }
        return false;
    }

    // Get all entities (for systems to query)
    std::vector<std::unique_ptr<Entity>>& getEntities();

    // Lightweight view: collect pointers to requested components for entities that have them.
    template<typename... Components>
    std::vector<std::tuple<Components*...>> view();

    // Component accessors (contiguous storage)
    template <typename T>
    T& addComponentToEntity(Entity* entity, EntityID id) { return componentManager.addComponent<T>(id); }
    template <typename T>
    T& getComponentForEntity(EntityID id) { return componentManager.getComponent<T>(id); }
    template <typename T>
    bool hasComponentForEntity(EntityID id) { return componentManager.hasComponent<T>(id); }
    EntityID getEntityID(Entity* e) { return entityIDs[e]; }

private:
    std::vector<std::unique_ptr<Entity>> entities;
    std::array<std::vector<Entity*>, maxGroups> groupedEntities;
    std::vector<std::unique_ptr<System>> systems;
    std::unordered_map<Entity*, EntityID> entityIDs;
    EntityID nextID = 0;
    ComponentManager componentManager;
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

// Implementation of view<Components...>()
#include <tuple>

template<typename... Components>
std::vector<std::tuple<Components*...>> Manager::view() {
    std::vector<std::tuple<Components*...>> result;
    result.reserve(128);

    for (auto& u : getEntities()) {
        Entity* e = u.get();
        if (!e->isActive()) continue;
        // check all components are present
        if ((e->hasComponent<Components>() && ...)) {
            result.emplace_back(&e->getComponent<Components>()...);
        }
    }
    return result;
}
