#pragma once

#include <stdio.h>

#include "../Vector2D.hpp"
#include "Components.hpp"

// Position, rotation and scale
class TransformComponent : public Component {
 public:
  Vector2D position;
  Vector2D velocity;

  int height = 16;
  int width = 16;
  int scale = 1;

  TransformComponent() { position.Zero(); }

  TransformComponent(int sc) {
    // position.Zero();
    position.x = 400;
    position.y = 320;
    scale = sc;
  }

  TransformComponent(float x, float y) {
    position.x = x;
    position.y = y;
  }

  TransformComponent(float x, float y, int h, int w, int sc) {
    position.x = x;
    position.y = y;
    height = h;
    width = w;
    scale = sc;
  }

  void setPosition(float x, float y) {
    position.x = x;
    position.y = y;
  }

  void init() override {
    velocity.x = 0;
    velocity.y = 0;
  }

  void update(float deltaTime) override {
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
    // std::cout << "Velocity: " << velocity.x << ", " << velocity.y <<
    // std::endl;
  }
};