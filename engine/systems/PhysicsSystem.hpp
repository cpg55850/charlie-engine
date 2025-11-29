#pragma once

#include "System.hpp"
#include "../core/Manager.hpp"
#include "../../include/scripts/Components.hpp"

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
        auto list = manager.each<PhysicsComponent, TransformComponent>();
        for (auto& tpl : list) {
            PhysicsComponent* physics = std::get<0>(tpl);
            TransformComponent* transform = std::get<1>(tpl);

            if (physics->isStatic) continue;

            if (physics->useGravity) {
                transform->velocity.y += gravity * physics->gravityScale * deltaTime;
            }

            transform->velocity.x *= physics->friction;
            transform->velocity.y *= physics->friction;
        }
    }
};
