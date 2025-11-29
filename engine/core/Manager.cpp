#include "Manager.hpp"
#include <algorithm>
#include <iostream>

void Manager::update(float deltaTime) {
    // Update all systems (they query entities themselves)
    for (auto& system : systems) {
        system->update(*this, deltaTime);
    }
    // Entity::update removed (entities are passive data containers now)
}

void Manager::render(float deltaTime) {
    for (auto& system : systems) {
        try {
            system->render(*this, deltaTime);
        } catch (const std::exception& e) {
            std::cerr << "Manager::render: system render threw: " << e.what() << "\n";
        }
    }
}

void Manager::shutdown() {
    for (auto& system : systems) {
        try {
            system->quit();
        } catch (const std::exception& e) {
            std::cerr << "Manager::shutdown: system quit threw: " << e.what() << "\n";
        }
    }
}

void Manager::refreshGroups() {
    for (size_t i = 0; i < maxGroups; ++i) {
        auto& group = groupedEntities[i];
        group.erase(std::remove_if(group.begin(), group.end(),
                                   [i](Entity* e) { return !e->isActive() || !e->hasGroup(i); }),
                    group.end());
    }
}

void Manager::removeFromAllGroups(Entity* entity) {
    for (size_t i = 0; i < maxGroups; ++i) {
        auto& group = groupedEntities[i];
        group.erase(std::remove(group.begin(), group.end(), entity), group.end());
    }
}


void Manager::refresh() {
    refreshGroups();
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
