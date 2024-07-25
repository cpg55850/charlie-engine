#pragma once
#include <cmath>
#include <iostream>

#include "../ECS/Animation.h"
#include "../ECS/Components.h"
#include "../Game.h"

class BulletComponent : public Component {
 public:
  void init() override {
    // Custom initialization code for CustomScript
    std::cout << "Hi from bullet" << std::endl;
    entity->addComponent<TransformComponent>(8);
    entity->addComponent<SpriteComponent>();
    entity->getComponent<SpriteComponent>().addTex(
        "assets/bullet.png", true, Animation(0, 1, 100), "bulletIdle");
  }

  void update() override {
    // Custom update logic for CustomScript
    // std::cout << "CustomScript updated!" << std::endl;
    entity->getComponent<SpriteComponent>().playTex("assets/bullet.png",
                                                    "bulletIdle");
    // Get the number of milliseconds since SDL initialization
    Uint32 ticks = SDL_GetTicks();

    // Use the sin function with a floating-point value
    double x = sin(
        ticks * 0.01);  // Multiply by 0.001 to convert milliseconds to seconds

    int intX =
        static_cast<int>(x * 100);  // Example scaling and conversion to int

    // std::cout << x << std::endl;
    entity->getComponent<TransformComponent>().setPosition(intX, 0);
  }

  void draw() override {
    // Custom draw logic for CustomScript
    // std::cout << "CustomScript drawn!" << std::endl;
  }
};