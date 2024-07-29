#pragma once

#include <cmath>
#include <iostream>
#include <random>
#include <string>

#include "../ECS/Animation.hpp"
#include "../ECS/Components.hpp"
#include "../Game.hpp"

class WallComponent : public Component {
 public:
  void init() override {
    std::cout
        << "Hi from wall"
        << std::endl;  // Initialize components only if not already present
    if (!entity->hasComponent<TransformComponent>()) {
      entity->addComponent<TransformComponent>(400, 600, 16, 16, 4);
    }

    if (!entity->hasComponent<SpriteComponent>()) {
      auto& sprite = entity->addComponent<SpriteComponent>();
      sprite.addTex("assets/wall.png", true, Animation(0, 1, 100), "wall");
      sprite.playTex("assets/wall.png", "wall");
    }

    if (!entity->hasComponent<ColliderComponent>()) {
      auto& collider = entity->addComponent<ColliderComponent>("wall");
    }
  }

  void update(float deltaTime) override {
    auto& sprite = entity->getComponent<SpriteComponent>();
    sprite.playTex("assets/wall.png", "wall");
  }

  void draw() override {
    // Drawing logic for BulletComponent, if needed
  }

  // ~BulletComponent() { std::cout << "Bye!" << std::endl; }
};
