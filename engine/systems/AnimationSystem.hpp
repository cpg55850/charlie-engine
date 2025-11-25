#pragma once

#include "../core/System.hpp"
#include "../core/Manager.hpp"
#include "../components/AnimatedSpriteComponent.hpp"
#include "../components/AnimationStateComponent.hpp"
#include "../components/TransformComponent.hpp"
#include <SDL.h>

// System that handles sprite animations
// Chooses animations based on AnimationStateComponent (direction, movement state)
class AnimationSystem : public System {
public:
    void update(Manager& manager, float deltaTime) override {
        for (auto& entity : manager.getEntities()) {
            if (!entity->isActive()) continue;

            // Process entities with both AnimatedSprite and AnimationState
            if (entity->hasComponent<AnimatedSpriteComponent>() &&
                entity->hasComponent<AnimationStateComponent>()) {

                auto& animSprite = entity->getComponent<AnimatedSpriteComponent>();
                auto& animState = entity->getComponent<AnimationStateComponent>();

                // System chooses which animation to play based on state
                selectAnimation(animSprite, animState);

                // Update the animation frame
                animSprite.update(deltaTime);
            }
            // Fallback: entities without AnimationStateComponent still animate
            else if (entity->hasComponent<AnimatedSpriteComponent>()) {
                auto& animSprite = entity->getComponent<AnimatedSpriteComponent>();
                animSprite.update(deltaTime);
            }
        }
    }

private:
    void selectAnimation(AnimatedSpriteComponent& sprite, AnimationStateComponent& state) {
        // System logic: Choose animation based on state

        if (!state.isMoving) {
            // Idle animations (if you have them)
            // sprite.playTex("assets/idle.png");
            return;
        }

        // Choose animation based on direction
        switch (state.facingDirection) {
            case AnimationStateComponent::UP:
                sprite.spriteFlip = SDL_FLIP_NONE;
                sprite.playTex("assets/walk-up.png");
                break;

            case AnimationStateComponent::DOWN:
                sprite.spriteFlip = SDL_FLIP_VERTICAL;
                sprite.playTex("assets/walk-up.png");
                break;

            case AnimationStateComponent::LEFT:
                sprite.spriteFlip = SDL_FLIP_HORIZONTAL;
                sprite.playTex("assets/walk-right.png");
                break;

            case AnimationStateComponent::RIGHT:
                sprite.spriteFlip = SDL_FLIP_NONE;
                sprite.playTex("assets/walk-right.png");
                break;

            case AnimationStateComponent::NONE:
            default:
                // Keep current animation
                break;
        }
    }
};

