#pragma once

#include "System.hpp"
#include "Manager.hpp"
#include "Components.hpp"

// Physics component for entities with physics properties
class PhysicsComponent : public Component {
public:
    float mass = 1.0f;
    float friction = 0.98f;  // 0.0 = no friction, 1.0 = full friction
    float bounciness = 0.0f; // 0.0 = no bounce, 1.0 = perfect bounce
    float gravityScale = 1.0f;
    bool useGravity = false;
    bool isStatic = false;   // Static objects don't move

    PhysicsComponent() = default;
    PhysicsComponent(float m, bool gravity = false)
        : mass(m), useGravity(gravity) {}
};

// System that applies physics forces and constraints
class PhysicsSystem : public System {
public:
    float gravity = 980.0f; // Gravity in units per second squared (9.8 m/s^2 * 100)

    void update(Manager& manager, float deltaTime) override {
        for (auto& entity : manager.getEntities()) {
            if (!entity->isActive()) continue;

            // Process entities with both Physics and Transform components
            if (entity->hasComponent<PhysicsComponent>() &&
                entity->hasComponent<TransformComponent>()) {

                auto& physics = entity->getComponent<PhysicsComponent>();
                auto& transform = entity->getComponent<TransformComponent>();

                // Skip static objects
                if (physics.isStatic) continue;

                // Apply gravity
                if (physics.useGravity) {
                    transform.velocity.y += gravity * physics.gravityScale * deltaTime;
                }

                // Apply friction (reduces velocity over time)
                transform.velocity.x *= physics.friction;
                transform.velocity.y *= physics.friction;
            }
        }
    }
};

