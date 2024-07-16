#pragma once

#include <stdio.h>

#include "../Vector2D.h"
#include "Components.h"

// Position, rotation and scale
class TransformComponent : public Component {
 public:
  Vector2D position;
  Vector2D velocity;

  // int speed = 3;

  TransformComponent() {
    position.x = 0;
    position.y = 0;
  }

  TransformComponent(float x, float y) {
    position.x = x;
    position.y = y;
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