#pragma once

#include <cmath>
#include <iostream>
#include <random>
#include <string>

#include "../ECS/Animation.h"
#include "../ECS/Components.h"
#include "../Game.h"

class BulletComponent : public Component {
 public:
  void init() override {
    std::cout << "Hi from bullet" << std::endl;

    // Ensure the TransformComponent and SpriteComponent are added only once
    if (!entity->hasComponent<TransformComponent>()) {
      entity->addComponent<TransformComponent>(8);
    }
    if (!entity->hasComponent<SpriteComponent>()) {
      entity->addComponent<SpriteComponent>();
      auto& sprite = entity->getComponent<SpriteComponent>();
      sprite.addTex("assets/bullet.png", true, Animation(0, 1, 100),
                    "bulletIdle");
      sprite.playTex("assets/bullet.png", "daf");
    }
  }

  void update() override {
    // Generate randomness only if needed
    static std::random_device rd;
    static std::mt19937 eng(rd());
    static std::uniform_int_distribution<> distr_int(1, 9999);

    int random_int = distr_int(eng);
    std::string str = std::to_string(random_int);
    const char* cstr = str.c_str();

    // Update sprite texture animation
    auto& sprite = entity->getComponent<SpriteComponent>();
    sprite.playTex("assets/bullet.png", cstr);

    // Use SDL timing for movement calculation
    Uint32 ticks = SDL_GetTicks();
    double x = sin(ticks * 0.01);

    int intX = entity->getComponent<TransformComponent>().position.x +
               static_cast<int>(x * 10);

    // Update position
    auto& transform = entity->getComponent<TransformComponent>();
    transform.setPosition(intX, transform.position.y);
  }

  void draw() override {
    // Drawing logic for BulletComponent, if needed
  }

  // ~BulletComponent() { std::cout << "Bye!" << std::endl; }
};
