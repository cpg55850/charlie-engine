#pragma once

#include <iostream>

#include "../ECS/Animation.hpp"
#include "../ECS/Components.hpp"
#include "../Game.hpp"

class PlayerComponent : public Component {
 public:
  enum Direction { UP, DOWN, LEFT, RIGHT };
  Direction currentDirection;
  bool hitSomething = false;
  bool collisionDetected = false;
  float speed = 8.0f;
  float dx = 0.0f;
  float dy = 0.0f;
  float bulletSpeed = 10.0f;

  void init() override;
  void update(float deltaTime) override;
  void movePlayer(float deltaTime);
  void shootable();
  void draw() override;
  void handleInput();

  virtual ~PlayerComponent();

 private:
  Uint32 lastShootTime;
};
