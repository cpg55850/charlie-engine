#pragma once

#include <iostream>

#include "../ECS/Animation.hpp"
#include "../ECS/Components.hpp"
#include "../Game.hpp"

class PlayerComponent : public Component {
 public:
  float speed = 200.0f;  // Units per second (works better with deltaTime)

  void init() override;
  void update(float deltaTime) override;
  void shootable();
  void draw() override;
  void handleInput();

  virtual ~PlayerComponent();
};
