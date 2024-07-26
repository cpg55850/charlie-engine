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
    // Custom initialization code for CustomScript
    std::cout << "Hi from bullet" << std::endl;
    entity->addComponent<TransformComponent>(8);
    entity->addComponent<SpriteComponent>();
    entity->getComponent<SpriteComponent>().addTex(
        "assets/bullet.png", true, Animation(0, 1, 100), "bulletIdle");
    entity->getComponent<SpriteComponent>().playTex("assets/bullet.png", "daf");
  }

  void update() override {
    std::random_device rd;   // Obtain a random number from hardware
    std::mt19937 eng(rd());  // Seed the engine with the random number
    std::uniform_int_distribution<> distr_int(1, 9999);  // Range [1, 10]
    int random_int = distr_int(eng);
    std::string str = std::to_string(random_int);
    const char* cstr = str.c_str();
    // Custom update logic for CustomScript
    // std::cout << "CustomScript updated!" << std::endl;
    entity->getComponent<SpriteComponent>().playTex("assets/bullet.png", cstr);
    // Get the number of milliseconds since SDL initialization
    Uint32 ticks = SDL_GetTicks();

    // Use the sin function with a floating-point value
    double x = sin(
        ticks * 0.01);  // Multiply by 0.001 to convert milliseconds to seconds

    int intX =
        entity->getComponent<TransformComponent>().position.x +
        static_cast<int>(x * 10);  // Example scaling and conversion to int

    // std::cout << x << std::endl;
    entity->getComponent<TransformComponent>().setPosition(
        intX, entity->getComponent<TransformComponent>().position.y);
  }

  void draw() override {
    // Custom draw logic for CustomScript
    // std::cout << "bullet!" << std::endl;
  }

  // ~BulletComponent() { std::cout << "Bye!" << std::endl; }
};