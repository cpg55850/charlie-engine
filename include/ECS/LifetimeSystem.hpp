#pragma once

#include "System.hpp"
#include "Manager.hpp"
#include "LifetimeComponent.hpp"
#include "TransformComponent.hpp"
#include <iostream>

// System that manages entity lifetimes
// Destroys entities that live too long or go off-screen
class LifetimeSystem : public System {
public:
    void update(Manager& manager, float deltaTime) override {
        for (auto& entity : manager.getEntities()) {
            if (!entity->isActive()) continue;

            if (entity->hasComponent<LifetimeComponent>()) {
                auto& lifetime = entity->getComponent<LifetimeComponent>();

                // Update lifetime counter
                lifetime.currentLifetime += deltaTime;

                // Check if entity has lived too long
                if (lifetime.currentLifetime >= lifetime.maxLifetime) {
                    entity->destroy();
                    continue;
                }

                // Check if entity is off-screen (if it has a transform)
                if (lifetime.destroyOffScreen && entity->hasComponent<TransformComponent>()) {
                    auto& transform = entity->getComponent<TransformComponent>();

                    // Simple bounds check - destroy if too far from origin
                    if (std::abs(transform.position.x) > lifetime.screenBounds ||
                        std::abs(transform.position.y) > lifetime.screenBounds) {
                        entity->destroy();
                    }
                }
            }
        }
    }
};

