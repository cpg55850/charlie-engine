#pragma once

#include <iostream>

#include "../ECS/Animation.hpp"
#include "../ECS/Components.hpp"
#include "../Game.hpp"

class PlayerComponent : public Component {
 public:
  bool wasPressed = false;

  void init() override {
    std::cout << "PlayerComponent initialized!" << std::endl;
    std::cout << "Entity address: " << entity << std::endl;

    // Initialize components only if not already present
    if (!entity->hasComponent<TransformComponent>()) {
      entity->addComponent<TransformComponent>(4);
    }

    if (!entity->hasComponent<SpriteComponent>()) {
      auto& sprite = entity->addComponent<SpriteComponent>();
      sprite.addTex("assets/walk-right.png", true, Animation(0, 2, 100),
                    "WalkX");
      sprite.addTex("assets/walk-up.png", true, Animation(0, 2, 100), "WalkUp");
      sprite.addTex("assets/walk-down.png", true, Animation(0, 2, 100),
                    "WalkDown");
      sprite.playTex("assets/walk-right.png", "WalkX");
    }

    entity->addComponent<KeyboardController>(SDL_GetKeyboardState(NULL));
    entity->addComponent<ColliderComponent>("player");
  }

  void update() override {
    auto& transform = entity->getComponent<TransformComponent>();
    auto& sprite = entity->getComponent<SpriteComponent>();

    int xAxis = transform.velocity.x;
    int yAxis = transform.velocity.y;

    if (xAxis != 0) {
      sprite.playTex("assets/walk-right.png", "WalkX");
    }

    if (yAxis != 0) {
      sprite.playTex(yAxis < 0 ? "assets/walk-up.png" : "assets/walk-down.png",
                     yAxis < 0 ? "WalkUp" : "WalkDown");
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);
    bool isPressed = state[SDL_SCANCODE_SPACE];

    if (isPressed && !wasPressed) {
      std::cout << "Bullet fired!" << std::endl;
      auto& bullet = Game::manager.addEntity();
      bullet.addGroup(Game::groupLabels::groupEnemies);
      bullet.addComponent<BulletComponent>();
      auto& bulletTransform = bullet.getComponent<TransformComponent>();
      bulletTransform.setPosition(transform.position.x, transform.position.y);
      // Game::audioManager.playAudio("laser", 0, -1);
    }

    wasPressed = isPressed;
  }

  void draw() override {
    // Drawing logic, if needed
  }
};
