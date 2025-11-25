#pragma once
// Game-specific AnimationSystem with hardcoded asset paths
#include "../ECS/Components.hpp"
#include "../../engine/core/System.hpp"
#include "../../engine/core/Manager.hpp"
#include <SDL.h>

class AnimationSystem : public System {
public:
    void update(Manager& manager, float dt) override {
        for (auto& e : manager.getEntities()) {
            if (!e->isActive()) continue;
            if (e->hasComponent<AnimatedSpriteComponent>() && e->hasComponent<AnimationStateComponent>()) {
                auto& sprite = e->getComponent<AnimatedSpriteComponent>();
                auto& state = e->getComponent<AnimationStateComponent>();
                select(sprite,state);
                sprite.update(dt);
            } else if (e->hasComponent<AnimatedSpriteComponent>()) {
                e->getComponent<AnimatedSpriteComponent>().update(dt);
            }
        }
    }
private:
    void select(AnimatedSpriteComponent& sprite, AnimationStateComponent& state) {
        if (!state.isMoving) return; // keep current
        switch (state.facingDirection) {
            case AnimationStateComponent::UP:
                sprite.spriteFlip = SDL_FLIP_NONE; sprite.playTex("assets/walk-up.png"); break;
            case AnimationStateComponent::DOWN:
                sprite.spriteFlip = SDL_FLIP_VERTICAL; sprite.playTex("assets/walk-up.png"); break;
            case AnimationStateComponent::LEFT:
                sprite.spriteFlip = SDL_FLIP_HORIZONTAL; sprite.playTex("assets/walk-right.png"); break;
            case AnimationStateComponent::RIGHT:
                sprite.spriteFlip = SDL_FLIP_NONE; sprite.playTex("assets/walk-right.png"); break;
            default: break;
        }
    }
};
