#pragma once

#include <SDL.h>

#include <string>

#include "Components.hpp"
#include "ECS.hpp"

class ColliderComponent : public Component {
 public:
  SDL_Rect collider;
  std::string tag;

  TransformComponent* transform;

  ColliderComponent(std::string t) { tag = t; }

  void init() override {
    // Commented because this was causing an error
    // if (!entity->hasComponent<TransformComponent>()) {
    //   entity->addComponent<TransformComponent>();
    // }
    transform = &entity->getComponent<TransformComponent>();

    Game::colliders.push_back(this);
  }

  void update(float deltaTime) override {
    collider.x = static_cast<int>(transform->position.x);
    collider.y = static_cast<int>(transform->position.y);
    collider.w = transform->width * transform->scale;
    collider.h = transform->height * transform->scale;
  }
};