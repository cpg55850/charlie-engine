#pragma once

#include <iostream>
#include <random>
#include <filesystem>

#include "../components//PlayerComponent.hpp"
#include "../../engine/components/Animation.hpp"
#include "../scripts/Components.hpp"
#include "../Game.hpp"
#include "../../engine/utils/Vector2D.hpp"
#include "../../engine/utils/EntityUtils.hpp"

class EnemyComponent : public Component {
 public:
  int health = 50;
  float speed = 100.0f;
  bool chasePlayer = true;

  // Shooting behavior (data only) - AI system will operate these
  float shootCooldown = 1.2f;       // seconds between possible shots (average)
  float shootTimer = 0.0f;          // accumulator
  float shootChance = 0.6f;         // chance to fire when cooldown expires (0..1) — higher = more frequent shooting
  float projectileSpeed = 300.0f;   // speed of enemy bullets

  void init() override {
    // Keep initialization minimal; visuals and colliders should still be ensured by spawner
    Entity& e = *entity;
    auto& transform = ensureComponent<TransformComponent>(e, 200, 400, 32, 32, 4);
    auto& sprite = ensureComponent<SpriteComponent>(e);
    // Ensure the enemy texture is set so the RenderSystem can draw it
    sprite.setTex("assets/enemy.png");
    ensureComponent<ColliderComponent>(e, "enemy");
    ensureComponent<FlashOnHitComponent>(e);

    // Debug: print current working directory and check asset existence
    try {
      auto cwd = std::filesystem::current_path();
      std::cout << "EnemyComponent::init cwd=" << cwd << "\n";
      std::filesystem::path p = cwd / "assets/enemy.png";
      std::cout << "Enemy asset exists: " << (std::filesystem::exists(p) ? "yes" : "no") << " -> " << p << "\n";
    } catch (...) {
      std::cout << "EnemyComponent::init: unable to query filesystem\n";
    }

    std::cout << "Enemy transform pos=(" << transform.position.x << "," << transform.position.y << ") scale=" << transform.scale << "\n";
  }

  // No behavior in the component itself - AI systems drive movement and shooting
  void draw() override {}
};
