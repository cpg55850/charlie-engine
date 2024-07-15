#pragma once

#include "Components.h"

// Position, rotation and scale
class TransformComponent : public Component {
 private:
  int xPos = 0;
  int yPos = 0;

 public:
  TransformComponent() {
    xPos = 0;
    yPos = 0;
  }

  TransformComponent(int x, int y) {
    xPos = x;
    yPos = y;
  }

  int x() { return xPos; }
  int y() { return yPos; }

  void init() override {
    xPos = 0;
    yPos = 0;
  }
  void update() override {
    xPos++;
    yPos++;
  }
  void setPosition(int x, int y) {
    xPos = x;
    yPos = y;
  }
};