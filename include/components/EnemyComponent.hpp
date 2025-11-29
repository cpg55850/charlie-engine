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
#include "../../engine/components/RenderComponent.hpp"
#include "../../engine/core/EventBus.hpp"

// Include projectile/damage components for collision handling
#include "../components/ProjectileComponent.hpp"
#include "../components/DamageComponent.hpp"

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
    // Ensure enemy has a RenderComponent to control ordering
    auto& rend = ensureComponent<RenderComponent>(e, engine::render::RenderLayer::Entities, 5);
    rend.visible = true;
    // Also set sprite texture and fallback ordering
    sprite.layer = engine::render::RenderLayer::Entities;
    sprite.zOffset = 5;
    ensureComponent<ColliderComponent>(e, "enemy");
    ensureComponent<FlashOnHitComponent>(e);

    // Subscribe to OnHitEvent to apply damage via event bus
    using engine::events::GetEventBus;
    GetEventBus().subscribe<engine::events::OnHitEvent>([this](std::shared_ptr<engine::events::OnHitEvent> ev) {
        if (!ev) return;
        if (ev->target != this->entity) return;
        this->health -= ev->damage;
        std::cout << "Enemy hit via EventBus! Health=" << this->health << "\n";
        if (this->health <= 0) this->entity->destroy();
    });


    std::cout << "Enemy transform pos=(" << transform.position.x << "," << transform.position.y << ") scale=" << transform.scale << "\n";
  }

  // No behavior in the component itself - AI systems drive movement and shooting
  void draw() override {}

  // // React to collisions
  // void onCollision(const CollisionEvent& event) override {
  //   if (!event.other) return;
  //   if (event.other->hasComponent<ProjectileComponent>()) {
  //     auto& pc = event.other->getComponent<ProjectileComponent>();
  //     if (pc.owner == this->entity) return; // ignore self-hit from own projectile
  //
  //     int dmg = 1;
  //     if (event.other->hasComponent<DamageComponent>()) dmg = event.other->getComponent<DamageComponent>().damage;
  //
  //     health -= dmg;
  //     std::cout << "Enemy hit! Health: " << health << "\n";
  //     if (health <= 0) entity->destroy();
  //
  //     // Destroy projectile
  //     event.other->destroy();
  //   }
  // }
};
