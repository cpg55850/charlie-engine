#pragma once

#include <iostream>
#include <cmath>

#include "../components//PlayerComponent.hpp"
#include "../../engine/components/Animation.hpp"
#include "../scripts/Components.hpp"
#include "../Game.hpp"
#include "../../engine/utils/Vector2D.hpp"

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
