#pragma once

#include <array>
#include <bitset>
#include <memory>
#include <stdexcept>
#include <vector>
#include "ECSConfig.hpp" // Includes ComponentID, Group, etc.

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

private:
    Manager& manager; // Reference to the manager that owns this entity
    bool active = true;
    std::vector<std::unique_ptr<Component>> components;

    // ECS-specific storage for components and groups
    ComponentArray componentArray;
    ComponentBitset componentBitSet;
    GroupBitset groupBitset;
};

// Implementation for templated methods

template <typename T>
bool Entity::hasComponent() const {
    return componentBitSet[getComponentTypeID<T>()];
}

template <typename T, typename... TArgs>
T& Entity::addComponent(TArgs&&... mArgs) {
    auto c = std::make_unique<T>(std::forward<TArgs>(mArgs)...);
    c->entity = this;
    T* rawPtr = c.get();
    components.emplace_back(std::move(c));
    componentArray[getComponentTypeID<T>()] = rawPtr;
    componentBitSet[getComponentTypeID<T>()] = true;
    rawPtr->init();
    return *rawPtr;
}

template <typename T>
T& Entity::getComponent() const {
    T* ptr = static_cast<T*>(componentArray[getComponentTypeID<T>()]);
    if (!ptr) {
        throw std::runtime_error("Component not found");
    }
    return *ptr;
}