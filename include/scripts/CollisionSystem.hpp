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
            int projCount = 0;
            int playerCount = 0;
            for (auto& e : entities) {
                Entity* en = e.get();
                if (!en || !en->isActive()) continue;
                if (en->hasComponent<ColliderComponent>()) {
                    auto& cc = en->getComponent<ColliderComponent>();
                    if (cc.tag == std::string("projectile")) ++projCount;
                    if (cc.tag == std::string("player")) ++playerCount;
                }
            }
            // std::cerr << "Collision debug: projectiles=" << projCount << " players=" << playerCount << " entities=" << entities.size() << "\n";
        }

        for (auto& e : entities) {
            if (!e->isActive()) continue;
            if (!e->hasComponent<TransformComponent>() || !e->hasComponent<ColliderComponent>()) continue;
            auto& tr = e->getComponent<TransformComponent>();
            float newX = tr.position.x + tr.velocity.x * dt;
            float newY = tr.position.y + tr.velocity.y * dt;
            SDL_Rect predicted{static_cast<int>(newX), static_cast<int>(newY), static_cast<int>(tr.width * tr.scale), static_cast<int>(tr.height * tr.scale)};
            if (hitsWall(predicted)) {
                tr.velocity.x = 0.0f; tr.velocity.y = 0.0f;
            }
        }
    }

protected:
    // We no longer handle per-component damage here; engine dispatches events to components.
    void onOverlap(class Entity* /*a*/, class Entity* /*b*/) override {
        // Intentionally empty: components handle collision responses via onCollision callbacks
    }

private:
    bool hitsWall(const SDL_Rect& predicted) {
        for (auto* cc : Game::colliders) {
            if (cc->tag == std::string("wall") && Collision::AABB(predicted, cc->collider)) return true;
        }
        return false;
    }
};
