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

  void draw() override {
    // Save the current draw color
    Uint8 r, g, b, a;
    SDL_GetRenderDrawColor(Game::renderer, &r, &g, &b, &a);

    // Set the draw color to red and draw the collider
    SDL_SetRenderDrawColor(Game::renderer, 255, 0, 0, 255);

    // Adjust the collider position based on the camera position
    SDL_Rect cameraRelativeCollider = {collider.x - Game::camera.getX() + 450,
                                       collider.y - Game::camera.getY() + 250,
                                       collider.w, collider.h};

    SDL_RenderDrawRect(Game::renderer, &cameraRelativeCollider);

    // Restore the original draw color
    SDL_SetRenderDrawColor(Game::renderer, r, g, b, a);
  }
};