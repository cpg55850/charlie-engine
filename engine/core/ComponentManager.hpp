#pragma once

#include <memory>
#include <unordered_map>
#include <typeindex>
#include "ECSConfig.hpp"
#include "ComponentArray.hpp"

// Manages all component arrays
class ComponentManager {
private:
    // Map from type to component array
    std::unordered_map<std::type_index, std::shared_ptr<IComponentArray>> componentArrays;

    // Map from type to component ID
    std::unordered_map<std::type_index, ComponentID> componentTypes;

    // Next component type ID
    ComponentID nextComponentType = 0;

    // Get component array for type T
    template<typename T>
    std::shared_ptr<ComponentPool<T>> getComponentArray() {
        std::type_index typeIndex(typeid(T));

        if (componentArrays.find(typeIndex) == componentArrays.end()) {
            // Register component type if not already registered
            registerComponent<T>();
        }

        return std::static_pointer_cast<ComponentPool<T>>(componentArrays[typeIndex]);
    }

public:
    // Register a component type
    template<typename T>
    void registerComponent() {
        std::type_index typeIndex(typeid(T));

        if (componentArrays.find(typeIndex) != componentArrays.end()) {
            return; // Already registered
        }

        componentTypes[typeIndex] = nextComponentType;
        componentArrays[typeIndex] = std::make_shared<ComponentPool<T>>();
        nextComponentType++;
    }

    // Get component type ID
    template<typename T>
    ComponentID getComponentType() {
        std::type_index typeIndex(typeid(T));
        return componentTypes[typeIndex];
    }

    // Add component to entity
    template<typename T>
    T& addComponent(EntityID entity) {
        return getComponentArray<T>()->insert(entity);
    }

    // Remove component from entity
    template<typename T>
    void removeComponent(EntityID entity) {
        getComponentArray<T>()->remove(entity);
    }

    // Get component for entity
    template<typename T>
    T& getComponent(EntityID entity) {
        return getComponentArray<T>()->get(entity);
    }

    // Check if entity has component
    template<typename T>
    bool hasComponent(EntityID entity) {
        return getComponentArray<T>()->has(entity);
    }

    // Get component array for iteration
    template<typename T>
    ComponentPool<T>& getAllComponents() {
        std::type_index typeIndex(typeid(T));

        if (componentArrays.find(typeIndex) == componentArrays.end()) {
            registerComponent<T>();
        }

        return *std::static_pointer_cast<ComponentPool<T>>(componentArrays[typeIndex]);
    }

    // Called when entity is destroyed
    void entityDestroyed(EntityID entity) {
        // Notify all component arrays
        for (auto& pair : componentArrays) {
            pair.second->entityDestroyed(entity);
        }
    }
};

