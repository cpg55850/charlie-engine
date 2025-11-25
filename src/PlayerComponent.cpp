//
// Created by Charlie Graham on 11/23/25.
//
// Refactored to work with ECS Systems:
// - InputSystem handles input state
// - MovementSystem updates position based on velocity
// - AnimationSystem updates sprite frames
// - CollisionSystem detects collisions
// - This script only sets velocity and handles game logic

#include "Collision.hpp"
#include "scripts/PlayerComponent.hpp"
#include "ECS/AnimatedSpriteComponent.hpp"
#include "ECS/AnimationStateComponent.hpp"
#include "ECS/ColliderComponent.hpp"
#include "ECS/CombatComponent.hpp"
#include "ECS/InputComponent.hpp"
#include "ECS/TransformComponent.hpp"
#include "ECS/EntityUtils.hpp"
#include <iostream>

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
