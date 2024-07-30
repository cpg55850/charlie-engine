#pragma once

#include <iostream>

#include "../ECS/Animation.hpp"
#include "../ECS/Components.hpp"

class EnemyComponent : public Component {
 public:
  void init() override {
    std::cout << "Hello from enemy" << std::endl;
    // Initialize components only if not already present
    if (!entity->hasComponent<TransformComponent>()) {
      entity->addComponent<TransformComponent>(200, 800, 32, 32, 4);
    }
    if (!entity->hasComponent<SpriteComponent>()) {
      auto& sprite = entity->addComponent<SpriteComponent>();
      sprite.setTex("assets/enemy.png");
    }
  }

  void update(float deltaTime) override {}

  void draw() override {
    // Drawing logic, if needed
  }

 private:
};
