#pragma once
#include <iostream>

#include "../Game.hpp"
#include "Animation.hpp"
#include "Components.hpp"

class ScriptComponent : public Component {
 public:
  void init() override {
    // Custom initialization code for CustomScript
    std::cout << "CustomScript initialized!" << std::endl;
    std::cout << entity << std::endl;

    entity->addComponent<TransformComponent>(4);
    // entity->addComponent<SpriteComponent>("assets/idle.png", false);
    // entity->addComponent<SpriteComponent>("assets/walk-right.png", true);
    // "assets/walk-right.png", true, Animation(0, 2, 100), "Walk"
    entity->addComponent<SpriteComponent>();
    entity->getComponent<SpriteComponent>().addTex(
        "assets/walk-right.png", true, Animation(0, 2, 100), "WalkX");
    entity->getComponent<SpriteComponent>().addTex(
        "assets/walk-up.png", true, Animation(0, 2, 100), "WalkUp");
    entity->getComponent<SpriteComponent>().addTex(
        "assets/walk-down.png", true, Animation(0, 2, 100), "WalkDown");
    entity->getComponent<SpriteComponent>().playTex("assets/walk-right.png",
                                                    "WalkX");
    const Uint8* state = SDL_GetKeyboardState(NULL);
    entity->addComponent<KeyboardController>(state);
    entity->addComponent<ColliderComponent>("player");
  }

  void update(float deltaTime) override {
    // Custom update logic for CustomScript
    // std::cout << "CustomScript updated!" << std::endl;
    // Player Update
    int xAxis = entity->getComponent<TransformComponent>().velocity.x;
    int yAxis = entity->getComponent<TransformComponent>().velocity.y;

    if (xAxis > 0) {
      std::cout << "Going right" << std::endl;
      entity->getComponent<SpriteComponent>().playTex("assets/walk-right.png",
                                                      "WalkX");
    } else if (xAxis < 0) {
      std::cout << "Going left" << std::endl;
      entity->getComponent<SpriteComponent>().playTex("assets/walk-right.png",
                                                      "WalkX");
    };

    if (yAxis < 0) {
      std::cout << "Going up" << std::endl;
      entity->getComponent<SpriteComponent>().playTex("assets/walk-up.png",
                                                      "WalkUp");
    } else if (yAxis > 0) {
      std::cout << "Going down" << std::endl;
      entity->getComponent<SpriteComponent>().playTex("assets/walk-down.png",
                                                      "WalkDown");
    };
  }

  void draw() override {
    // Custom draw logic for CustomScript
    // std::cout << "CustomScript drawn!" << std::endl;
  }
};