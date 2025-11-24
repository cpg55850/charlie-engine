#include "ECS/Entity.hpp"
#include "ECS/Manager.hpp"
#include <stdexcept>

// Constructor
Entity::Entity(Manager& manager) : manager(manager), active(true) {
    componentArray.fill(nullptr); // Ensure ComponentArray is initialized
    componentBitSet.reset();      // Reset the bitset for components
}

// Update
void Entity::update(float deltaTime) {
    if (!active) return;
    for (auto& c : components) {
        if (c) c->update(deltaTime);
    }
}

// Draw
void Entity::draw() {
    if (!active) return;
    for (auto& c : components) {
        if (c) c->draw();
    }
}

// Destroy
void Entity::destroy() {
    active = false;
}

// Group Management
bool Entity::hasGroup(Group mGroup) const {
    return groupBitset[mGroup];
}

void Entity::addGroup(Group mGroup) {
    groupBitset[mGroup] = true;
    manager.addToGroup(this, mGroup);
}

void Entity::delGroup(Group mGroup) {
    groupBitset[mGroup] = false;
}

bool Entity::isActive() const {
    return active;
}