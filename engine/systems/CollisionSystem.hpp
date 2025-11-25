#pragma once

#include "../core/System.hpp"
#include "../core/Manager.hpp"
#include "../../include/scripts/Components.hpp"
#include "../SDLCore/Collision.hpp"
#include "../../include/Game.hpp"
#include "../../include/components/EnemyComponent.hpp"

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
        auto& colliderA = entityA->getComponent<ColliderComponent>();
        auto& colliderB = entityB->getComponent<ColliderComponent>();
        // Bullet hits enemy
        if ((colliderA.tag == "projectile" && colliderB.tag == "enemy") ||
            (colliderB.tag == "projectile" && colliderA.tag == "enemy")) {
            std::cout << "CollisionSystem: projectile hit enemy" << std::endl;
            Entity* bullet = (colliderA.tag == "projectile") ? entityA : entityB;
            Entity* enemy  = (colliderA.tag == "enemy") ? entityA : entityB;
            if (enemy->hasComponent<FlashOnHitComponent>()) {
                auto& flash = enemy->getComponent<FlashOnHitComponent>();
                flash.trigger();
            }
            int dmg = 1;
            if (bullet->hasComponent<DamageComponent>()) {
                dmg = bullet->getComponent<DamageComponent>().damage;
            }
            if (enemy->hasComponent<EnemyComponent>()) {
                auto& ec = enemy->getComponent<EnemyComponent>();
                ec.health -= dmg;
                std::cout << "Enemy health now: " << ec.health << std::endl;
                if (ec.health <= 0) {
                    std::cout << "Enemy destroyed" << std::endl;
                    enemy->destroy();
                }
            }
            bullet->destroy();
            return; // done
        }
    }
};
