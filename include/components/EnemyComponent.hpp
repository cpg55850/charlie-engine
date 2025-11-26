#pragma once

#include <iostream>
#include <cmath>

#include "../components//PlayerComponent.hpp"
#include "../../engine/components/Animation.hpp"
#include "../scripts/Components.hpp"
#include "../Game.hpp"
#include "../../engine/utils/Vector2D.hpp"
#include "../../engine/utils/EntityUtils.hpp"

class EnemyComponent : public Component {
 public:
  int health = 5;
  float speed = 100.0f;
  bool chasePlayer = true;

  void init() override {
    std::cout << "Hello from enemy" << std::endl;
    Entity& e = *entity;
    // Initialize required components using ensureComponent helper
    auto& transform = ensureComponent<TransformComponent>(e, 200, 400, 32, 32, 4);
    auto& sprite    = ensureComponent<SpriteComponent>(e);
    sprite.setTex("assets/enemy.png");  // Always ensure correct texture (safe override)
    // if (!e.hasComponent<ColliderComponent>()) {
    //     e.addComponent<ColliderComponent>("enemy");
    // }
    ensureComponent<ColliderComponent>(e, "enemy");
    ensureComponent<FlashOnHitComponent>(e);
  }

  void update(float) override { /* AI handled in EnemyAISystem */ }
  void draw() override {}

 private:
};
