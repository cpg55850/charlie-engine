#pragma once

#include "../core/System.hpp"
#include "../core/Manager.hpp"
#include "../../include/components/ScriptComponent.hpp"
#include "../../include/components/PlayerComponent.hpp"

// System that handles script components - custom game logic
class ScriptSystem : public System {
public:
    void update(Manager& manager, float deltaTime) override {
        // Update PlayerComponent explicitly (primary gameplay logic)
        auto players = manager.view<PlayerComponent>();
        for (auto& tpl : players) {
            PlayerComponent* pc = std::get<0>(tpl);
            Entity* owner = pc->entity;
            if (!owner || !owner->isActive()) continue;
            pc->update(deltaTime);
        }

        // Update pure ScriptComponent wrappers
        auto scripts = manager.view<ScriptComponent>();
        for (auto& tpl : scripts) {
            ScriptComponent* sc = std::get<0>(tpl);
            Entity* owner = sc->entity;
            if (!owner || !owner->isActive()) continue;
            sc->update(deltaTime);
        }
    }
};
