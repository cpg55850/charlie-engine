#include "ECS/Manager.hpp"
#include <algorithm>

void Manager::update(float deltaTime) {
    // Update all systems (they query entities themselves)
    for (auto& system : systems) {
        system->update(*this, deltaTime);
    }

    // Update entities (for backwards compatibility with component update methods)
    for (auto& entity : entities) {
        entity->update(deltaTime);
    }
}

void Manager::draw() {
    for (const auto& entity : entities) entity->draw();
}

void Manager::refresh() {
    for (size_t i = 0; i < maxGroups; ++i) {
        auto& group = groupedEntities[i];
        group.erase(std::remove_if(group.begin(), group.end(),
                                   [i](Entity* e) { return !e->isActive() || !e->hasGroup(i); }),
                    group.end());
    }
    entities.erase(std::remove_if(entities.begin(), entities.end(),
                                  [](const std::unique_ptr<Entity>& e) { return !e->isActive(); }),
                   entities.end());
}

Entity& Manager::addEntity() {
    auto entity = std::make_unique<Entity>(*this);
    Entity& ref = *entity;
    entities.emplace_back(std::move(entity));
    return ref;
}

void Manager::addToGroup(Entity* entity, Group group) {
    groupedEntities[group].emplace_back(entity);
}

std::vector<Entity*>& Manager::getGroup(Group group) {
    return groupedEntities[group];
}

std::vector<std::unique_ptr<Entity>>& Manager::getEntities() {
    return entities;
}
