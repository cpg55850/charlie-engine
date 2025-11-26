#pragma once

#include <iostream>

#include "../../engine/core/Component.hpp"
#include "../../engine/components/Animation.hpp"
#include "../scripts/Components.hpp"
#include "../Game.hpp"

struct PlayerComponent : public Component {
  float speed = 400.0f;  // Units per second (works better with deltaTime)
  int health = 10;       // health in HP; default 10 -> 10 hits of damage=1 kills
  int ammo = 0;          // optional ammo

  PlayerComponent() = default;
  PlayerComponent(float s, int h = 10, int a = 0) : speed(s), health(h), ammo(a) {}

  void init() override;  // implemented in .cpp to attach required components
};
