#pragma once

#include "../Game.hpp"
#include "Components.hpp"
#include "ECS.hpp"
#include "stdio.h"

class KeyboardController : public Component {
 public:
  TransformComponent* transform;
  SpriteComponent* sprite;
  float hSpeed = 0.0f;
  float vSpeed = 0.0f;
  const uint8_t* mState;

  KeyboardController(const uint8_t* state) {
    mState = state;
    std::cout << "mState set to " << state << std::endl;
  }

  void init() override {
    transform = &entity->getComponent<TransformComponent>();
    sprite = &entity->getComponent<SpriteComponent>();
  }

  void update(float deltaTime) override {
    hSpeed = 0;
    vSpeed = 0;

    sprite->play("Idle");

    if (mState[SDL_SCANCODE_D] || mState[SDL_SCANCODE_RIGHT]) {
      hSpeed += 10.0f;
      sprite->play("Walk");
      sprite->spriteFlip = SDL_FLIP_NONE;
    }
    if (mState[SDL_SCANCODE_A] || mState[SDL_SCANCODE_LEFT]) {
      hSpeed -= 10.0f;
      sprite->play("Walk");
      sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
    }
    // up/down
    if (mState[SDL_SCANCODE_S] || mState[SDL_SCANCODE_DOWN]) {
      vSpeed += 10.0f;
      sprite->play("Walk");
      sprite->spriteFlip = SDL_FLIP_HORIZONTAL;
    }
    if (mState[SDL_SCANCODE_W] || mState[SDL_SCANCODE_UP]) {
      vSpeed -= 10.0f;
      sprite->play("Walk");
      sprite->spriteFlip = SDL_FLIP_NONE;
    }

    transform->velocity.x = hSpeed;
    transform->velocity.y = vSpeed;
  }
};