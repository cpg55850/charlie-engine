#pragma once

#include <iostream>

#include "../ECS/Animation.hpp"
#include "../ECS/Components.hpp"
#include "../Game.hpp"

class PlayerComponent : public Component {
 public:
  bool hitSomething = false;
  bool collisionDetected = false;
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
    // Store the original position
    auto& transform = entity->getComponent<TransformComponent>();
    float originalX = transform.position.x;
    float originalY = transform.position.y;

    movePlayer(deltaTime, collisionDetected);

    // Calculate the new position
    float newX = transform.position.x + dx * deltaTime;
    float newY = transform.position.y + dy * deltaTime;

    // Create a temporary collider with the new position
    SDL_Rect tempCollider = {static_cast<int>(newX), static_cast<int>(newY),
                             transform.width * transform.scale,
                             transform.height * transform.scale};

    // Check collisions with all walls
    for (auto cc : Game::colliders) {
      if (cc->tag == "wall" && Collision::AABB(tempCollider, cc->collider)) {
        collisionDetected = true;
        std::cout << dx << " " << dy << std::endl;
        std::cout << "Hit something!" << std::endl;
        break;  // Exit the loop if a collision is detected
      } else {
        collisionDetected = false;
      }
    }

    // Move the player as close to the wall as possible without colliding
    if (collisionDetected) {
      // Step size for incremental movement
      float stepSize = 0.1f;

      // Move incrementally until just before collision
      while (true) {
        // Calculate the next incremental position
        float nextX = transform.position.x + (dx * stepSize);
        float nextY = transform.position.y + (dy * stepSize);

        // Create a temporary collider with the next incremental position
        SDL_Rect nextTempCollider = {static_cast<int>(nextX),
                                     static_cast<int>(nextY),
                                     transform.width * transform.scale,
                                     transform.height * transform.scale};

        // Check for collision at the next incremental position
        bool willCollide = false;
        for (auto cc : Game::colliders) {
          if (cc->tag == "wall" &&
              Collision::AABB(nextTempCollider, cc->collider)) {
            willCollide = true;
            break;
          }
        }

        // If a collision will occur, stop moving
        if (willCollide) {
          break;
        }

        // Update the position to the next incremental position
        transform.position.x = nextX;
        transform.position.y = nextY;
      }
    } else {
      // Update the position only if no collision is detected
      transform.position.x = newX;
      transform.position.y = newY;
    }
  }

  void draw() override {
    // Drawing logic, if needed
  }

  void movePlayer(float deltaTime, bool collisionDetected) {
    auto& sprite = entity->getComponent<SpriteComponent>();

    sprite.play("Idle");

    const Uint8* mState = SDL_GetKeyboardState(NULL);
    std::cout << "got to here" << std::endl;
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

    // if (collisionDetected) {
    //   dx = -dx;
    //   dy = -dy;
    // }
  }

 private:
};
