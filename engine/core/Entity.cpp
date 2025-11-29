#include "Entity.hpp"
#include "Manager.hpp"
#include <stdexcept>

// Constructor
Entity::Entity(Manager& manager) : manager(manager), active(true) {
    componentBitSet.reset();      // Reset the bitset for components
    groupBitset.reset();
    componentArray.fill(nullptr); // initialize restored array
}

// Update (no-op in pure ECS; systems drive behavior)
void Entity::update(float /*deltaTime*/) {
    if (!active) return;
    for (auto& c : components) if (c) c->update(0.f); // call component updates (deltaTime ignored placeholder)
}

// Draw (no-op; RenderSystem handles drawing)
void Entity::draw() {
    if (!active) return;
    for (auto& c : components) if (c) c->draw();
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