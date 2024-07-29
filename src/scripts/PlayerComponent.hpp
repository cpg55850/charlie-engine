#pragma once

#include <iostream>

#include "../ECS/Animation.hpp"
#include "../ECS/Components.hpp"
#include "../Game.hpp"

class PlayerComponent : public Component {
 public:
  bool hitSomething = false;
  float speed = 10.0f;
  float dx = 0.0f;
  float dy = 0.0f;

  void init() override {
    std::cout << "PlayerComponent initialized!" << std::endl;
    std::cout << "Entity address: " << entity << std::endl;

    // Initialize components only if not already present
    if (!entity->hasComponent<TransformComponent>()) {
      entity->addComponent<TransformComponent>(600, 600, 16, 16, 4);
    }

    if (!entity->hasComponent<SpriteComponent>()) {
      auto& sprite = entity->addComponent<SpriteComponent>();
      sprite.addTex("assets/walk-right.png", true, Animation(0, 2, 100),
                    "WalkX");
      sprite.addTex("assets/walk-up.png", true, Animation(0, 2, 100), "WalkUp");
      sprite.addTex("assets/walk-down.png", true, Animation(0, 2, 100),
                    "WalkDown");
      sprite.playTex("assets/walk-right.png", "WalkX");
    }

    entity->addComponent<ColliderComponent>("player");
  }

  void update(float deltaTime) override {
    hitSomething = false;

    movePlayer(deltaTime);

    std::cout << dx << " " << dy << " " << deltaTime << std::endl;

    for (auto cc : Game::colliders) {
      if (Collision::AABB(entity->getComponent<ColliderComponent>(), *cc) &&
          cc->tag == "wall") {
        // resolveCollision(entity->getComponent<TransformComponent>(),
        //                  entity->getComponent<ColliderComponent>().collider,
        //                  cc->collider, deltaTime);

        hitSomething = true;
      }
    }

    if (!hitSomething) {
      // If no collision, update position normally
      entity->getComponent<TransformComponent>().position.x += dx * deltaTime;
      entity->getComponent<TransformComponent>().position.y += dy * deltaTime;
    } else {
      entity->getComponent<TransformComponent>().position.x -= dx * deltaTime;
      entity->getComponent<TransformComponent>().position.y -= dy * deltaTime;
    }
  }

  void draw() override {
    // Drawing logic, if needed
  }

  void movePlayer(float deltaTime) {
    auto& sprite = entity->getComponent<SpriteComponent>();

    sprite.play("Idle");

    const Uint8* mState = SDL_GetKeyboardState(NULL);

    dx = 0;
    dy = 0;

    if (mState[SDL_SCANCODE_D] || mState[SDL_SCANCODE_RIGHT]) {
      dx = speed;
      sprite.spriteFlip = SDL_FLIP_NONE;
    }
    if (mState[SDL_SCANCODE_A] || mState[SDL_SCANCODE_LEFT]) {
      dx = -speed;
      sprite.spriteFlip = SDL_FLIP_HORIZONTAL;
    }
    if (mState[SDL_SCANCODE_W] || mState[SDL_SCANCODE_UP]) {
      dy = -speed;
    }
    if (mState[SDL_SCANCODE_S] || mState[SDL_SCANCODE_DOWN]) {
      dy = speed;
    }
  }

  void resolveCollision(TransformComponent& transform,
                        const SDL_Rect& playerCollider,
                        const SDL_Rect& wallCollider, float deltaTime) {
    // Calculate the overlap on each axis
    int overlapX = (playerCollider.x + playerCollider.w / 2) -
                   (wallCollider.x + wallCollider.w / 2);
    int overlapY = (playerCollider.y + playerCollider.h / 2) -
                   (wallCollider.y + wallCollider.h / 2);

    // Determine the smallest overlap
    if (std::abs(overlapX) < std::abs(overlapY)) {
      // Resolve collision on the X axis
      if (overlapX > 0) {
        transform.position.x = wallCollider.x + wallCollider.w;
      } else {
        transform.position.x = wallCollider.x - playerCollider.w;
      }
      // transform.velocity.x = 0;  // Stop horizontal movement
    } else {
      // Resolve collision on the Y axis
      if (overlapY > 0) {
        transform.position.y = wallCollider.y + wallCollider.h;
      } else {
        transform.position.y = wallCollider.y - playerCollider.h;
      }
      // transform.velocity.y = 0;  // Stop vertical movement
    }
  }

 private:
};
