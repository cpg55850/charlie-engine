#pragma once

#include "../Game.h"
#include "Components.h"
#include "ECS.h"
#include "stdio.h"

class KeyboardController : public Component {
 public:
  TransformComponent* transform;
  float hSpeed = 0.0f;
  float vSpeed = 0.0f;
  const uint8_t* mState;

  KeyboardController(const uint8_t* state) {
    mState = state;
    std::cout << "mState set to " << state << std::endl;
  }

  void init() override {
    transform = &entity->getComponent<TransformComponent>();
  }

  void update() override {
    hSpeed = 0;
    vSpeed = 0;

    if (mState[SDL_SCANCODE_D] || mState[SDL_SCANCODE_RIGHT]) {
      hSpeed += 10.0f;
    }
    if (mState[SDL_SCANCODE_A] || mState[SDL_SCANCODE_LEFT]) {
      hSpeed -= 10.0f;
    }
    // up/down
    if (mState[SDL_SCANCODE_S] || mState[SDL_SCANCODE_DOWN]) {
      vSpeed += 10.0f;
    }
    if (mState[SDL_SCANCODE_W] || mState[SDL_SCANCODE_UP]) {
      vSpeed -= 10.0f;
    }

    transform->velocity.x = hSpeed;
    transform->velocity.y = vSpeed;
  }
};