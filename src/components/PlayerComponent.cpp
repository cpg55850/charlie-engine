//
// Created by Charlie Graham on 11/23/25.
//
// Refactored to work with ECS Systems:
// - InputSystem handles input state
// - MovementSystem updates position based on velocity
// - AnimationSystem updates sprite frames
// - CollisionSystem detects collisions
// - This script only sets velocity and handles game logic

#include "../../engine/SDLCore/Collision.hpp"
#include "../../include/components/PlayerComponent.hpp"
#include "../../engine/components/AnimatedSpriteComponent.hpp"
#include "../../engine/components/AnimationStateComponent.hpp"
#include "../../engine/components/ColliderComponent.hpp"
#include "../../include/components/CombatComponent.hpp"
#include "../../engine/components/InputComponent.hpp"
#include "../../engine/components/TransformComponent.hpp"
#include "../../engine/utils/EntityUtils.hpp"
#include "../../engine/components/RenderComponent.hpp"
#include <iostream>

// Add missing component headers used in onCollision
#include "../../include/components/ProjectileComponent.hpp"
#include "../../include/components/DamageComponent.hpp"

// Legacy behavior moved to PlayerInputSystem; keep only initial component wiring helper
// If this file is not needed you can remove it entirely.

void PlayerComponent::init() {
    Entity& e = *entity;
    auto& transform = ensureComponent<TransformComponent>(e, 600, 600, 16, 16, 4);

    auto& sprite = ensureComponent<AnimatedSpriteComponent>(e);
    if (sprite.animations.empty()) { // only populate if first time
        sprite.addTex("assets/walk-right.png", Animation(0, 2, 100));
        sprite.addTex("assets/walk-up.png", Animation(0, 2, 100));
        sprite.addTex("assets/walk-down.png", Animation(0, 2, 100));
        sprite.playTex("assets/walk-right.png");
    }

    // Ensure player renders on Entities layer and slightly above tiles
    auto& render = ensureComponent<RenderComponent>(e, RenderLayer::Entities, 10);
    render.visible = true;

    ensureComponent<ColliderComponent>(e, "player");
    ensureComponent<InputComponent>(e);
    ensureComponent<AnimationStateComponent>(e);
    auto& combat = ensureComponent<CombatComponent>(e);
    if (combat.projectileSpeed == 0) { // configure defaults if newly added
        combat.projectileSpeed = 400.0f;
        combat.fireRate = 100.0f;
    }
    std::cout << "PlayerComponent::init wiring complete for entity " << &e << std::endl;
}

void PlayerComponent::onCollision(const CollisionEvent& event) {
    if (!event.other) return;
    // If other is a projectile and not owned by this player, take damage and destroy projectile
    if (event.other->hasComponent<ProjectileComponent>()) {
        auto& pc = event.other->getComponent<ProjectileComponent>();
        // Friendly fire check
        if (pc.owner == this->entity) return;

        int dmg = 1;
        if (event.other->hasComponent<DamageComponent>()) dmg = event.other->getComponent<DamageComponent>().damage;

        health -= dmg;
        std::cout << "Player hit! Health: " << health << "\n";

        if (health <= 0) entity->destroy();

        // Destroy the projectile
        event.other->destroy();

        // TODO: trigger HUD update via event or global state
    }
}
