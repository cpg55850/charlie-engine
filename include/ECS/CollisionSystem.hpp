#pragma once

#include "System.hpp"
#include "Manager.hpp"
#include "Components.hpp"
#include "../Collision.hpp"
#include "../Game.hpp"

// System that handles collision detection and response
// Prevents entities from moving into solid colliders
class CollisionSystem : public System {
public:
    void update(Manager& manager, float deltaTime) override {
        auto& entities = manager.getEntities();

        // First pass: Predict collisions and prevent movement into walls
        for (auto& entity : entities) {
            if (!entity->isActive()) continue;

            // Only check entities that have both transform and collider
            if (entity->hasComponent<TransformComponent>() &&
                entity->hasComponent<ColliderComponent>()) {

                auto& transform = entity->getComponent<TransformComponent>();
                auto& collider = entity->getComponent<ColliderComponent>();

                // Predict next position based on velocity
                float newX = transform.position.x + transform.velocity.x * deltaTime;
                float newY = transform.position.y + transform.velocity.y * deltaTime;

                SDL_Rect predictedCollider = {
                    static_cast<int>(newX),
                    static_cast<int>(newY),
                    transform.width * transform.scale,
                    transform.height * transform.scale
                };

                // Check against all static colliders (walls, obstacles)
                if (wouldCollideWithWall(predictedCollider)) {
                    // Stop movement by zeroing velocity
                    // This is the SYSTEM deciding how to respond to collision
                    transform.velocity.x = 0;
                    transform.velocity.y = 0;
                }
            }
        }

        // Second pass: Check entity-to-entity collisions (after movement)
        for (size_t i = 0; i < entities.size(); ++i) {
            Entity* entityA = entities[i].get();
            if (!entityA->isActive() || !entityA->hasComponent<ColliderComponent>()) {
                continue;
            }

            for (size_t j = i + 1; j < entities.size(); ++j) {
                Entity* entityB = entities[j].get();
                if (!entityB->isActive() || !entityB->hasComponent<ColliderComponent>()) {
                    continue;
                }

                // Check if the colliders overlap
                if (Collision::AABB(entityA->getComponent<ColliderComponent>(),
                                   entityB->getComponent<ColliderComponent>())) {
                    handleCollision(entityA, entityB);
                }
            }
        }
    }

private:
    // Check if predicted collider would hit a wall
    bool wouldCollideWithWall(const SDL_Rect& predictedCollider) {
        for (auto* cc : Game::colliders) {
            if (cc->tag == "wall" && Collision::AABB(predictedCollider, cc->collider)) {
                return true;
            }
        }
        return false;
    }

    void handleCollision(Entity* entityA, Entity* entityB) {
        // Entity-to-entity collision response
        // This is where you'd handle player hitting enemy, bullet hitting target, etc.
        // For now, detection only

        auto& colliderA = entityA->getComponent<ColliderComponent>();
        auto& colliderB = entityB->getComponent<ColliderComponent>();

        // Example: Could trigger events, damage, etc.
        // if (colliderA.tag == "player" && colliderB.tag == "enemy") { ... }
    }
};

