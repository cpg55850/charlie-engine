#pragma once

#include "System.hpp"
#include "Manager.hpp"
#include "ScriptComponent.hpp"

// System that handles script components - custom game logic
class ScriptSystem : public System {
public:
    void update(Manager& manager, float deltaTime) override {
        // Query entities from the manager each frame
        for (auto& entity : manager.getEntities()) {
            if (!entity->isActive()) continue;

            // Only process entities with ScriptComponent
            if (entity->hasComponent<ScriptComponent>()) {
                auto& script = entity->getComponent<ScriptComponent>();
                script.update(deltaTime);
            }
        }
    }
};

