#pragma once
#include "System.hpp"
#include "Manager.hpp"
#include "scripts/PlayerComponent.hpp"
#include "InputComponent.hpp"
#include "TransformComponent.hpp"
#include "AnimationStateComponent.hpp"
#include "CombatComponent.hpp"
#include "Vector2D.hpp"

class PlayerInputSystem : public System {
public:
    void update(Manager& manager, float dt) override {
        for (auto& e : manager.getEntities()) {
            if (!e->isActive()) continue;
            if (!e->hasComponent<PlayerComponent>()) continue;
            if (!e->hasComponent<InputComponent>() || !e->hasComponent<TransformComponent>() ||
                !e->hasComponent<AnimationStateComponent>() || !e->hasComponent<CombatComponent>()) {
                continue; // Require all needed components
            }
            auto& input = e->getComponent<InputComponent>();
            auto& transform = e->getComponent<TransformComponent>();
            auto& animState = e->getComponent<AnimationStateComponent>();
            auto& combat = e->getComponent<CombatComponent>();
            auto& player = e->getComponent<PlayerComponent>();

            // Reset per-frame movement intent
            transform.velocity = Vector2D(0,0);
            animState.isMoving = false;

            if (input.isPressed("MoveRight")) {
                animState.facingDirection = AnimationStateComponent::RIGHT;
                animState.isMoving = true;
                transform.velocity.x = player.speed;
            }
            if (input.isPressed("MoveLeft")) {
                animState.facingDirection = AnimationStateComponent::LEFT;
                animState.isMoving = true;
                transform.velocity.x = -player.speed;
            }
            if (input.isPressed("MoveUp")) {
                animState.facingDirection = AnimationStateComponent::UP;
                animState.isMoving = true;
                transform.velocity.y = -player.speed;
            }
            if (input.isPressed("MoveDown")) {
                animState.facingDirection = AnimationStateComponent::DOWN;
                animState.isMoving = true;
                transform.velocity.y = player.speed;
            }

            // Shooting request routed to CombatSystem
            if (input.isPressed("Shoot")) {
                combat.requestShoot(animState.facingDirection);
            }
        }
    }
};
