#pragma once

#include <cmath>
#include <iostream>
#include <random>
#include <string>

#include "../../engine/components/Animation.hpp"
#include "../scripts/Components.hpp"
#include "../Game.hpp"

class BulletComponent : public Component {
 private:
  Uint32 creationTime;
  Uint32 lifespan = 5000;  // 1 second in milliseconds

 public:
  void init() override {
    creationTime = SDL_GetTicks();
    std::cout << "Hi from bullet" << std::endl;

    Game::audioManager.playAudio("laser");

    // Ensure the TransformComponent and AnimatedSpriteComponent are added only
    // once
    if (!entity->hasComponent<TransformComponent>()) {
      entity->addComponent<TransformComponent>(8);
    }
    if (!entity->hasComponent<SpriteComponent>()) {
      entity->addComponent<SpriteComponent>();
      auto& sprite = entity->getComponent<SpriteComponent>();
      sprite.setTex("assets/bullet.png");
    }
  }

  void update(float deltaTime) override {
    if (SDL_GetTicks() - creationTime > lifespan) {
      entity->destroy();
    }
  }

  void draw() override {
    // Drawing logic for BulletComponent, if needed
  }

  ~BulletComponent() { std::cout << "Bye!" << std::endl; }
};
