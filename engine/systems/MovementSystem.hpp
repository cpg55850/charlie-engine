#pragma once

#include "../core/System.hpp"
#include "../core/Manager.hpp"
#include "../components/TransformComponent.hpp"
#include "../components/ColliderComponent.hpp"

// System that handles movement by updating entity positions based on velocity
class MovementSystem : public System {
public:
    void update(Manager& manager, float deltaTime) override {
        // Query entities from the manager each frame
        for (auto& entity : manager.getEntities()) {
            if (!entity->isActive()) continue;
            if (!entity->hasComponent<TransformComponent>()) continue;
            auto& transform = entity->getComponent<TransformComponent>();

            // Apply velocity
            transform.position.x += transform.velocity.x * deltaTime;
            transform.position.y += transform.velocity.y * deltaTime;

            // Sync collider if present
            if (entity->hasComponent<ColliderComponent>()) {
                auto& col = entity->getComponent<ColliderComponent>();
                col.collider.x = static_cast<int>(transform.position.x);
                col.collider.y = static_cast<int>(transform.position.y);
                col.collider.w = transform.width * transform.scale;
                col.collider.h = transform.height * transform.scale;
            }

            // Update sprite (static)
            if (entity->hasComponent<SpriteComponent>()) {
                entity->getComponent<SpriteComponent>().update(deltaTime);
            }

            // Update animated sprite frame rects
            if (entity->hasComponent<AnimatedSpriteComponent>()) {
                entity->getComponent<AnimatedSpriteComponent>().update(deltaTime);
            }
        }
    }
};
