#pragma once

#include <stdio.h>

#include "../Vector2D.h"
#include "Components.h"

// Position, rotation and scale
class TransformComponent : public Component {
 public:
  Vector2D position;
  Vector2D velocity;

  int height = 32;
  int width = 32;
  int scale = 1;

  // int speed = 3;

  TransformComponent() {
    position.x = 0;
    position.y = 0;
  }

  TransformComponent(float x, float y) {
    position.x = x;
    position.y = y;
  }

  TransformComponent(int sc) {
    position.x = 0.0f;
    position.y = 0.0f;
    scale = sc;
  }

  TransformComponent(float x, float y, int h, int w, int sc) {
    position.x = x;
    position.y = y;
    height = h;
    width = w;
    scale = sc;
  }

  void init() override {
    velocity.x = 0;
    velocity.y = 0;
  }

  void update() override {
    position.x += velocity.x;
    position.y += velocity.y;
    // std::cout << "Velocity: " << velocity.x << ", " << velocity.y <<
    // std::endl;
  }
};