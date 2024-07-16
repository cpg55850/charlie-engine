#pragma once

#include "../Game.h"
#include "Components.h"
#include "ECS.h"
#include "stdio.h"

class KeyboardController : public Component {
 public:
  TransformComponent *transform;

  void init() override {
    transform = &entity->getComponent<TransformComponent>();
  }

  void update() override {
    if (Game::event.type == SDL_KEYDOWN) {
      switch (Game::event.key.keysym.sym) {
        case SDLK_w:
          transform->velocity.y = -1;
          std::cout << "Pressing Up: " << transform->velocity.x << std::endl;

          break;
        case SDLK_a:
          transform->velocity.x = -1;
          break;
        case SDLK_d:
          transform->velocity.x = 1;
          break;
        case SDLK_s:
          transform->velocity.y = 1;
          break;
      }
    }

    if (Game::event.type == SDL_KEYUP) {
      switch (Game::event.key.keysym.sym) {
        case SDLK_w:
          transform->velocity.y = 0;
          break;
        case SDLK_a:
          transform->velocity.x = 0;
          break;
        case SDLK_d:
          transform->velocity.x = 0;
          break;
        case SDLK_s:
          transform->velocity.y = 0;
          break;
      }
    }
  }
};