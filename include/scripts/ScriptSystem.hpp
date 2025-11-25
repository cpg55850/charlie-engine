#pragma once
// Game-specific ScriptSystem using PlayerComponent
#include "../../engine/core/System.hpp"
#include "../../engine/core/Manager.hpp"
#include "Components.hpp"
#include "../components/PlayerComponent.hpp"

class ScriptSystem : public System {
public:
    void update(Manager& manager, float dt) override {
        for (auto& e : manager.getEntities()) {
            if (!e->isActive()) continue;
            if (e->hasComponent<PlayerComponent>()) {
                e->getComponent<PlayerComponent>().update(dt);
            }
            if (e->hasComponent<ScriptComponent>()) {
                e->getComponent<ScriptComponent>().update(dt);
            }
        }
    }
};
