#pragma once

#include <iostream>
#include <cmath>

#include "PlayerComponent.hpp"
#include "../ECS/Animation.hpp"
#include "../ECS/Components.hpp"
#include "../Game.hpp"
#include "../Vector2D.hpp"

class EnemyComponent : public Component {
 public:
  int health = 5;
  float speed = 100.0f;
  bool chasePlayer = true;

  void init() override {
    std::cout << "Hello from enemy" << std::endl;
    // Initialize components only if not already present
    if (!entity->hasComponent<TransformComponent>()) {
      entity->addComponent<TransformComponent>(200, 400, 32, 32, 4);
    }
    if (!entity->hasComponent<SpriteComponent>()) {
      auto& sprite = entity->addComponent<SpriteComponent>();
      sprite.setTex("assets/enemy.png");
    }
    if (!entity->hasComponent<ColliderComponent>()) {
      entity->addComponent<ColliderComponent>("enemy");
    }
    if (!entity->hasComponent<FlashOnHitComponent>()) {
      entity->addComponent<FlashOnHitComponent>();
    }
  }

  void update(float) override { /* AI handled in EnemyAISystem */ }
  void draw() override {}

 private:
};
