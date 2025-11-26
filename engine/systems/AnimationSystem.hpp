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
        // First, process entities that have both AnimatedSpriteComponent and AnimationStateComponent
        auto both = manager.view<AnimatedSpriteComponent, AnimationStateComponent>();
        for (auto& tpl : both) {
            AnimatedSpriteComponent* animSprite = std::get<0>(tpl);
            AnimationStateComponent* animState = std::get<1>(tpl);
            selectAnimation(*animSprite, *animState);
            animSprite->update(deltaTime);
        }

        // Then process entities that have only AnimatedSpriteComponent (no state)
        auto onlyAnim = manager.view<AnimatedSpriteComponent>();
        for (auto& tpl : onlyAnim) {
            AnimatedSpriteComponent* animSprite = std::get<0>(tpl);
            // If the entity also had an AnimationStateComponent it was already processed above.
            animSprite->update(deltaTime);
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
