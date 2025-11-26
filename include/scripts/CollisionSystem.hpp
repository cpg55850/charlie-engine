#pragma once
// Game-specific CollisionSystem (uses Game::colliders, enemy combat logic)
#include "Components.hpp"
#include "../Game.hpp"
#include "../../engine/core/Manager.hpp"
#include "../../engine/systems/CollisionSystem.hpp" // inherit from engine system
#include "Collision.hpp"
#include "../components/EnemyComponent.hpp"
#include "../components/PlayerComponent.hpp"
#include <iostream>

#include "../components/ProjectileComponent.hpp"

// Rename to GameCollisionSystem to avoid conflicting type name with engine::CollisionSystem
class GameCollisionSystem : public ::CollisionSystem {
public:
    // Keep predictive wall collision specific to the game
    void update(Manager& manager, float dt) override {
        // First, run the engine-level behavior (sync colliders and detect overlaps)
        ::CollisionSystem::update(manager, dt);

        // Then run predictive wall collision logic (game specific)
        auto& entities = manager.getEntities();
        // Debug: every 60 frames print counts of projectiles/players to spot missing colliders
        static int debugFrame = 0;
        ++debugFrame;
        if ((debugFrame & 63) == 0) {
            int projCount = 0, playerCount = 0;
            for (auto& e : entities) {
                Entity* en = e.get();
                if (!en || !en->isActive()) continue;
                if (en->hasComponent<ColliderComponent>()) {
                    auto& cc = en->getComponent<ColliderComponent>();
                    if (cc.tag == "projectile") ++projCount;
                    if (cc.tag == "player") ++playerCount;
                }
            }
            std::cerr << "Collision debug: projectiles=" << projCount << " players=" << playerCount << " entities=" << entities.size() << "\n";
        }

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

        // Determine which entity is the projectile (bullet) and which is the target
        Entity* bullet = nullptr;
        Entity* target = nullptr;
        if (ca.tag == "projectile") { bullet = a; target = b; }
        else if (cb.tag == "projectile") { bullet = b; target = a; }
        else return; // not a projectile-involved collision

        // (No heavy logging in hot collision path) If you need collision debug, define
        // DEBUG_COLLISIONS and re-enable logging via the macro below.
#if defined(DEBUG_COLLISIONS)
        try {
            auto& bcol = bullet->getComponent<ColliderComponent>();
            auto& tcol = target->getComponent<ColliderComponent>();
            std::cerr << "Collision: bullet=" << bullet << " tag=" << bcol.tag
                      << " rect=(" << bcol.collider.x << "," << bcol.collider.y << "," << bcol.collider.w << "," << bcol.collider.h << ")"
                      << " target=" << target << " tag=" << tcol.tag
                      << " rect=(" << tcol.collider.x << "," << tcol.collider.y << "," << tcol.collider.w << "," << tcol.collider.h << ")\n";
        } catch (...) {
            std::cerr << "Collision: debug failed to read collider components\n";
        }
#endif

        // Determine damage early
        int dmg = bullet->hasComponent<DamageComponent>() ? bullet->getComponent<DamageComponent>().damage : 1;

        // If the projectile has an owner and that owner is the same as the target, ignore
        if (bullet->hasComponent<ProjectileComponent>()) {
            auto& pc = bullet->getComponent<ProjectileComponent>();
            if (pc.owner == target) return; // ignore self-hit
        }

        // If target is a player, apply damage and destroy projectile
        if (target->hasComponent<PlayerComponent>()) {
            auto& player = target->getComponent<PlayerComponent>();
            player.health -= dmg;
            std::cout << "player health now: " << player.health << std::endl;
            bullet->destroy();
            return;
        }

        // If target has FlashOnHit, trigger it
        if (target->hasComponent<FlashOnHitComponent>()) target->getComponent<FlashOnHitComponent>().trigger();

        // If target is an enemy, apply damage to enemy health
        if (target->hasComponent<EnemyComponent>()) {
            auto& ec = target->getComponent<EnemyComponent>();
            ec.health -= dmg;
            std::cout << "Enemy health now: " << ec.health << std::endl;
            if (ec.health <= 0) target->destroy();
        }

        // Destroy projectile after processing
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
