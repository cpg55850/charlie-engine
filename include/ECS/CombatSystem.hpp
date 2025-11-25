#pragma once

#include <iostream>
#include "System.hpp"
#include "Manager.hpp"
#include "CombatComponent.hpp"
#include "TransformComponent.hpp"
#include "SpriteComponent.hpp"
#include "../Game.hpp"

// System that handles combat - spawning projectiles, handling shoot requests
// This decouples shooting logic from player/enemy scripts
class CombatSystem : public System {
public:
    void update(Manager& manager, float deltaTime) override {
        // Process all entities with CombatComponent
        for (auto& entity : manager.getEntities()) {
            if (!entity->isActive()) continue;

            // Only process entities with combat capability
            if (entity->hasComponent<CombatComponent>() &&
                entity->hasComponent<TransformComponent>()) {

                auto& combat = entity->getComponent<CombatComponent>();
                auto& transform = entity->getComponent<TransformComponent>();

                // Check if entity requested to shoot and can shoot
                if (combat.shootRequested && combat.canShoot()) {
                    spawnProjectile(manager, transform, combat);
                    combat.markShot();
                }
            }
        }
    }

private:
    void spawnProjectile(Manager& manager, TransformComponent& shooterTransform,
                        CombatComponent& combat) {
        // std::cout << "CombatSystem: Spawning projectile..." << std::endl;
        // System creates the bullet entity
        auto& bullet = manager.addEntity();

        // Setup bullet transform at shooter's position
        auto& bulletTransform = bullet.addComponent<TransformComponent>(
            shooterTransform.position.x,
            shooterTransform.position.y,
            combat.projectileWidth,
            combat.projectileHeight,
            combat.projectileScale
        );
        // std::cout << "CombatSystem: Bullet transform initial pos (" << bulletTransform.position.x << ", " << bulletTransform.position.y << ")" << std::endl;

        // Calculate bullet velocity based on direction
        float velocityX = 0.0f;
        float velocityY = 0.0f;

        switch (combat.shootDirection) {
            case AnimationStateComponent::UP:
                velocityY = -combat.projectileSpeed;
                break;
            case AnimationStateComponent::DOWN:
                velocityY = combat.projectileSpeed;
                break;
            case AnimationStateComponent::LEFT:
                velocityX = -combat.projectileSpeed;
                break;
            case AnimationStateComponent::RIGHT:
                velocityX = combat.projectileSpeed;
                break;
            default:
                break;
        }

        // Inherit shooter's velocity (for moving platforms, etc.)
        bulletTransform.velocity.x = shooterTransform.velocity.x + velocityX;
        bulletTransform.velocity.y = shooterTransform.velocity.y + velocityY;

        // Add visual component
        auto& sprite = bullet.addComponent<SpriteComponent>();
        sprite.setTex(combat.projectileTexture);
        // std::cout << "CombatSystem: Bullet velocity (" << bulletTransform.velocity.x << ", " << bulletTransform.velocity.y << ")" << std::endl;

        // Add lifetime component - destroy bullet after 5 seconds or if off-screen
        bullet.addComponent<LifetimeComponent>(5.0f);

        // Add to projectiles group
        bullet.addGroup(Game::groupProjectiles);

        // Could add more components:
        // bullet.addComponent<ColliderComponent>("projectile");
        // bullet.addComponent<DamageComponent>(10);
    }
};
