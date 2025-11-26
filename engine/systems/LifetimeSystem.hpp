#pragma once

#include "../core/System.hpp"
#include "../core/Manager.hpp"
#include "../components/LifetimeComponent.hpp"
#include "../components/TransformComponent.hpp"
#include <iostream>

// System that manages entity lifetimes
// Destroys entities that live too long or go off-screen
class LifetimeSystem : public System {
public:
    void update(Manager& manager, float deltaTime) override {
        // View entities that have LifetimeComponent
        auto lifetimes = manager.view<LifetimeComponent>();
        for (auto& tpl : lifetimes) {
            LifetimeComponent* lifetime = std::get<0>(tpl);
            // lifetime->entity points to the owner; ensure it's active
            Entity* owner = lifetime->entity;
            if (!owner || !owner->isActive()) continue;

            lifetime->currentLifetime += deltaTime;
            if (lifetime->currentLifetime >= lifetime->maxLifetime) {
                owner->destroy();
                continue;
            }

            if (lifetime->destroyOffScreen && owner->hasComponent<TransformComponent>()) {
                auto& transform = owner->getComponent<TransformComponent>();
                if (std::abs(transform.position.x) > lifetime->screenBounds ||
                    std::abs(transform.position.y) > lifetime->screenBounds) {
                    owner->destroy();
                }
            }
        }
    }
};
