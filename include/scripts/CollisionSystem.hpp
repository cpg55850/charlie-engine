#pragma once
// Game-specific CollisionSystem (uses Game::colliders, enemy combat logic)
#include "Components.hpp"
#include "../Game.hpp"
#include "../../engine/core/Manager.hpp"
#include "../../engine/systems/CollisionSystem.hpp" // inherit from engine system
#include "Collision.hpp"
#include "../components/EnemyComponent.hpp"
#include <iostream>

// Rename to GameCollisionSystem to avoid conflicting type name with engine::CollisionSystem
class GameCollisionSystem : public ::CollisionSystem {
public:
    // Keep predictive wall collision specific to the game
    void update(Manager& manager, float dt) override {
        // First, run the engine-level behavior (sync colliders and detect overlaps)
        ::CollisionSystem::update(manager, dt);

        // Then run predictive wall collision logic (game specific)
        auto& entities = manager.getEntities();
        for (auto& e : entities) {
            if (!e->isActive()) continue;
            if (!e->hasComponent<TransformComponent>() || !e->hasComponent<ColliderComponent>()) continue;
            auto& tr = e->getComponent<TransformComponent>();
            float newX = tr.position.x + tr.velocity.x * dt;
            float newY = tr.position.y + tr.velocity.y * dt;
            SDL_Rect predicted{static_cast<int>(newX), static_cast<int>(newY), tr.width * tr.scale, tr.height * tr.scale};
            if (hitsWall(predicted)) {
                tr.velocity.x = 0; tr.velocity.y = 0;
            }
        }
    }

protected:
    // Override the engine hook to implement project-specific hit responses
    void onOverlap(class Entity* a, class Entity* b) override {
        auto& ca = a->getComponent<ColliderComponent>();
        auto& cb = b->getComponent<ColliderComponent>();

        // Determine which entity is the bullet and which is the enemy
        Entity* bullet = nullptr;
        Entity* enemy  = nullptr;

        if (ca.tag == "projectile" && cb.tag == "enemy") {
            bullet = a;
            enemy  = b;
        } else if (cb.tag == "projectile" && ca.tag == "enemy") {
            bullet = b;
            enemy  = a;
        } else {
            return; // not a bullet-enemy collision
        }

        if (enemy->hasComponent<FlashOnHitComponent>()) enemy->getComponent<FlashOnHitComponent>().trigger();
        int dmg = bullet->hasComponent<DamageComponent>() ? bullet->getComponent<DamageComponent>().damage : 1;
        if (enemy->hasComponent<EnemyComponent>()) {
            auto& ec = enemy->getComponent<EnemyComponent>();
            ec.health -= dmg;
            std::cout << "Enemy health now: " << ec.health << std::endl;
            if (ec.health <= 0) enemy->destroy();
        }
        bullet->destroy();
    }

private:
    bool hitsWall(const SDL_Rect& predicted) {
        for (auto* cc : Game::colliders) {
            if (cc->tag == "wall" && Collision::AABB(predicted, cc->collider)) return true;
        }
        return false;
    }
};
