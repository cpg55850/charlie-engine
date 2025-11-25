#pragma once

#include "../core/System.hpp"
#include "../core/Manager.hpp"
#include "../../include/components/ScriptComponent.hpp"
#include "../../include/components/PlayerComponent.hpp"

// System that handles script components - custom game logic
class ScriptSystem : public System {
public:
    void update(Manager& manager, float deltaTime) override {
        for (auto& entity : manager.getEntities()) {
            if (!entity->isActive()) continue;

            // Update PlayerComponent explicitly (primary gameplay logic)
            if (entity->hasComponent<PlayerComponent>()) {
                auto& pc = entity->getComponent<PlayerComponent>();
                pc.update(deltaTime);
            }
            // If any pure ScriptComponent wrappers exist, update them too
            if (entity->hasComponent<ScriptComponent>()) {
                auto& sc = entity->getComponent<ScriptComponent>();
                sc.update(deltaTime);
            }
        }
    }
};
