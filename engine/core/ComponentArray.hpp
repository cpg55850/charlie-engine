#pragma once

#include <vector>
#include <unordered_map>
#include <memory>
#include <functional>
#include "ECSConfig.hpp"

// Interface for type-erased component arrays
class IComponentArray {
public:
    virtual ~IComponentArray() = default;
    virtual void entityDestroyed(EntityID entity) = 0;
};

// Template class for storing components of type T in contiguous memory
template<typename T>
class ComponentPool : public IComponentArray {
private:
    std::vector<T> components;               // contiguous storage

    // Maps from entity ID to array index
    std::unordered_map<EntityID, size_t> entityToIndex;

    // Maps from array index to entity ID
    std::unordered_map<size_t, EntityID> indexToEntity;

public:
    // Add component for entity (returns uninitialized component - caller must construct it)
    T& insert(EntityID entity) {
        if (entityToIndex.find(entity) != entityToIndex.end()) {
            // Component already exists, return it
            return components[entityToIndex[entity]];
        }

        // Add new component at end of array
        size_t newIndex = components.size();
        entityToIndex[entity] = newIndex;
        indexToEntity[newIndex] = entity;

        components.emplace_back(); // default constructed
        return components.back();
    }

    // Remove component for entity
    void remove(EntityID entity) {
        auto it = entityToIndex.find(entity);
        if (it == entityToIndex.end()) return;

        size_t index = it->second;
        size_t lastIndex = components.size() - 1;

        // Swap removed component with last component (keeps array packed)
        if (index != lastIndex) {
            components[index] = std::move(components[lastIndex]);

            // Update mappings for swapped component
            EntityID movedEntity = indexToEntity[lastIndex];
            entityToIndex[movedEntity] = index;
            indexToEntity[index] = movedEntity;
        }

        // Remove mappings
        entityToIndex.erase(entity);
        indexToEntity.erase(lastIndex);

        components.pop_back(); // calls destructor of removed component
    }

    // Get component for entity
    T& get(EntityID entity) {
        return components[entityToIndex[entity]];
    }

    // Check if entity has component
    bool has(EntityID entity) const {
        return entityToIndex.find(entity) != entityToIndex.end();
    }

    // Get entity at specific index
    EntityID getEntityAt(size_t index) const {
        return indexToEntity.at(index);
    }

    // Get number of components
    size_t getSize() const {
        return components.size();
    }

    // Get raw contiguous data (for cache-friendly iteration)
    std::vector<T>& getData() {
        return components;
    }

    // Iterate over all components
    void forEach(std::function<void(T&, EntityID)> func) {
        for (size_t i = 0; i < components.size(); ++i) {
            func(components[i], indexToEntity[i]);
        }
    }

    // IComponentArray interface
    void entityDestroyed(EntityID entity) override {
        if (has(entity)) {
            remove(entity);
        }
    }
};
