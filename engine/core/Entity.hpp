#pragma once

#include <array>
#include <bitset>
#include <memory>
#include <stdexcept>
#include <vector>
#include "ECSConfig.hpp"

#include "Events.hpp" // for CollisionEvent

class Component;  // Forward declare Component class
class Manager;    // Forward declare Manager class

class Entity {
public:
    // Constructor - Initializes arrays and sets the manager
    explicit Entity(Manager& mManager);

    // Primary methods for the game loop
    void update(float deltaTime);
    void draw();
    bool isActive() const;
    void destroy();

    // Group functionality
    bool hasGroup(Group mGroup) const;
    void addGroup(Group mGroup);
    void delGroup(Group mGroup);

    // Component methods
    template <typename T>
    bool hasComponent() const;

    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs);

    template <typename T>
    T& getComponent() const;

    // New: dispatch a collision event to all components (components are private)
    void sendCollisionEvent(const CollisionEvent& ev);

private:
    Manager& manager;
    bool active = true;
    std::vector<std::unique_ptr<Component>> components; // restored
    ComponentArray componentArray{}; // restored pointer array
    ComponentBitset componentBitSet;
    GroupBitset groupBitset;
};

// Include template implementations after full class definition
#include "Entity.inl"

// Component template implementations require Entity to be complete
// (moved to Component.hpp to avoid include cycle)
