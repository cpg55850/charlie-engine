#include "ECS/Manager.hpp"
#include <algorithm>

void Manager::update(float deltaTime) {
    // Update all systems (they query entities themselves)
    for (auto& system : systems) {
        system->update(*this, deltaTime);
    }
    // Entity::update removed (entities are passive data containers now)
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
    // Remove dead entities and purge their components
    entities.erase(std::remove_if(entities.begin(), entities.end(),
                                  [&](const std::unique_ptr<Entity>& e) {
                                      if (!e->isActive()) {
                                          EntityID id = entityIDs[e.get()];
                                          componentManager.entityDestroyed(id);
                                          entityIDs.erase(e.get());
                                          return true;
                                      }
                                      return false;
                                  }),
                   entities.end());
}

Entity& Manager::addEntity() {
    auto entity = std::make_unique<Entity>(*this);
    Entity& ref = *entity;
    entityIDs[&ref] = nextID++;
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
