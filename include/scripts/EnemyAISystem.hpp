#pragma once
#include "../../engine/core/System.hpp"
#include "../../engine/core/Manager.hpp"
#include "../../engine/components/TransformComponent.hpp"
#include "../components/EnemyComponent.hpp"
#include "components/PlayerComponent.hpp"
#include <cmath>
#include <limits>
#include <random>
#include "../components/ProjectileComponent.hpp"

class EnemyAISystem : public System {
public:
    void update(Manager& manager, float dt) override {
        // Get player transforms once using the new view helper
        auto playerTuples = manager.each<TransformComponent, PlayerComponent>();
        if (playerTuples.empty()) return;

        // Build a compact list of player transforms for fast iteration
        std::vector<TransformComponent*> players;
        players.reserve(playerTuples.size());
        for (auto& tpl : playerTuples) players.push_back(std::get<0>(tpl));

        // RNG for shooting decisions
        static thread_local std::mt19937 rng{std::random_device{}()};
        std::uniform_real_distribution<float> uniform01(0.0f, 1.0f);
        // shorter jitter -> on average enemies will shoot more often
        std::uniform_real_distribution<float> cooldownJitter(0.8f, 1.6f);

        // Iterate enemy entities via view to get (Transform, Enemy) pairs directly
        auto enemyTuples = manager.each<TransformComponent, EnemyComponent>();
        for (auto& tpl : enemyTuples) {
            TransformComponent* et = std::get<0>(tpl);
            EnemyComponent* ec = std::get<1>(tpl);
            Entity* owner = et->entity;
            if (!owner || !owner->isActive()) continue;

            // Movement: chase nearest player
            if (ec->chasePlayer) {
                TransformComponent* nearest = nullptr;
                float nearestDist2 = std::numeric_limits<float>::max();
                for (auto* pt : players) {
                    float dx = pt->position.x - et->position.x;
                    float dy = pt->position.y - et->position.y;
                    float d2 = dx*dx + dy*dy;
                    if (d2 < nearestDist2) { nearestDist2 = d2; nearest = pt; }
                }

                if (!nearest) { et->velocity.x = et->velocity.y = 0.0f; }
                else {
                    float dx = nearest->position.x - et->position.x;
                    float dy = nearest->position.y - et->position.y;
                    float len = std::sqrt(dx*dx + dy*dy);
                    if (len > 1.0f) {
                        et->velocity.x = (dx / len) * ec->speed;
                        et->velocity.y = (dy / len) * ec->speed;
                    } else {
                        et->velocity.x = et->velocity.y = 0.0f; // reached
                    }
                }
            }

            // Shooting: update timer and possibly spawn projectile
            ec->shootTimer += dt;
            if (ec->shootTimer >= ec->shootCooldown) {
                if (uniform01(rng) < ec->shootChance) {
                    // target nearest player again (reuse players list)
                    TransformComponent* target = nullptr;
                    float nearestDist2 = std::numeric_limits<float>::max();
                    for (auto* pt : players) {
                        float dx = pt->position.x - et->position.x;
                        float dy = pt->position.y - et->position.y;
                        float d2 = dx*dx + dy*dy;
                        if (d2 < nearestDist2) { nearestDist2 = d2; target = pt; }
                    }
                    if (target) spawnProjectile(manager, owner, *et, *target, ec->projectileSpeed);
                }
                ec->shootTimer = 0.0f;
                ec->shootCooldown = cooldownJitter(rng);
            }
        }
    }

private:
    void spawnProjectile(Manager& manager, Entity* owner, const TransformComponent& origin,
                         const TransformComponent& target, float projSpeed) {
        auto& bullet = manager.addEntity();

        // Compute direction from origin center to target center
        const float originCenterX = origin.position.x + (origin.width * origin.scale) / 2.0f;
        const float originCenterY = origin.position.y + (origin.height * origin.scale) / 2.0f;
        const float targetCenterX = target.position.x + (target.width * target.scale) / 2.0f;
        const float targetCenterY = target.position.y + (target.height * target.scale) / 2.0f;
        float dx = targetCenterX - originCenterX;
        float dy = targetCenterY - originCenterY;
        float len = std::sqrt(dx*dx + dy*dy);
        float nx = 0.0f, ny = 0.0f;
        if (len > 1e-6f) { nx = dx / len; ny = dy / len; }

        // Place the bullet just outside the origin's collider along the firing direction
        const float bulletW = 8.0f, bulletH = 8.0f;
        const float halfOrigin = std::max(origin.width * origin.scale, origin.height * origin.scale) * 0.5f;
        const float halfBullet = std::max(bulletW, bulletH) * 0.5f;
        const float spawnDist = halfOrigin + halfBullet + 1.0f; // small epsilon to avoid overlap

        const float spawnX = originCenterX + nx * spawnDist - bulletW * 0.5f;
        const float spawnY = originCenterY + ny * spawnDist - bulletH * 0.5f;

        auto& bT = bullet.addComponent<TransformComponent>(spawnX, spawnY, static_cast<int>(bulletH), static_cast<int>(bulletW), 1);

        // Set velocity along normalized direction
        if (len > 1e-6f) {
            bT.velocity.x = nx * projSpeed;
            bT.velocity.y = ny * projSpeed;
        } else {
            // fallback: shoot upward
            bT.velocity.y = -projSpeed;
        }

        auto& sprite = bullet.addComponent<SpriteComponent>();
        sprite.setTex("assets/bullet.png");
        bullet.addComponent<ColliderComponent>("projectile");
        // mark owner so collision handling can ignore friendly collisions
        bullet.addComponent<ProjectileComponent>(owner);
        bullet.addComponent<DamageComponent>(1);
        bullet.addComponent<LifetimeComponent>(5.0f);
        bullet.addGroup(Game::groupProjectiles);
    }
};
