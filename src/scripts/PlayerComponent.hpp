#pragma once

#include <iostream>

#include "../ECS/Animation.hpp"
#include "../ECS/Components.hpp"
#include "../Game.hpp"
#include "../scripts/ScriptComponents.hpp"

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

  void init() override {
    std::cout << "PlayerComponent initialized!" << std::endl;
    std::cout << "Entity address: " << entity << std::endl;

    // Initialize components only if not already present
    if (!entity->hasComponent<TransformComponent>()) {
      entity->addComponent<TransformComponent>(600, 600, 16, 16, 4);
    }

    if (!entity->hasComponent<AnimatedSpriteComponent>()) {
      auto& sprite = entity->addComponent<AnimatedSpriteComponent>();
      sprite.addTex("assets/walk-right.png", Animation(0, 2, 100));
      sprite.addTex("assets/walk-up.png", Animation(0, 2, 100));
      sprite.addTex("assets/walk-down.png", Animation(0, 2, 100));
      sprite.playTex("assets/walk-right.png");
    }

    entity->addComponent<ColliderComponent>("player");
  }

  void update(float deltaTime) override {
    // Store the original position
    handleInput();
    shootable();
    movePlayer(deltaTime);

    // Reset velocity after movement
    auto& transform = entity->getComponent<TransformComponent>();
    transform.velocity.x = 0;
    transform.velocity.y = 0;
  }
  void movePlayer(float deltaTime) {
    auto& transform = entity->getComponent<TransformComponent>();

    // Calculate the new position based on velocity
    float newX = transform.position.x + transform.velocity.x * deltaTime;
    float newY = transform.position.y + transform.velocity.y * deltaTime;

    // Create a temporary collider with the new position
    SDL_Rect tempCollider = {static_cast<int>(newX), static_cast<int>(newY),
                             transform.width * transform.scale,
                             transform.height * transform.scale};

    // Check collisions with all walls
    bool collisionDetected = false;
    for (auto cc : Game::colliders) {
      if (cc->tag == "wall" && Collision::AABB(tempCollider, cc->collider)) {
        collisionDetected = true;
        std::cout << "Hit something!" << std::endl;
        break;  // Exit the loop if a collision is detected
      }
    }

    // Move the player as close to the wall as possible without colliding
    if (collisionDetected) {
      // Step size for incremental movement
      float stepSize = 0.1f;

      // Move incrementally until just before collision
      while (true) {
        // Calculate the next incremental velocity
        float nextVelocityX = transform.velocity.x * stepSize;
        float nextVelocityY = transform.velocity.y * stepSize;

        // Calculate the next incremental position
        float nextX = transform.position.x + nextVelocityX;
        float nextY = transform.position.y + nextVelocityY;

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

      // Reset velocity to zero after collision handling
      transform.velocity.x = 0;
      transform.velocity.y = 0;
    } else {
      // Apply the velocity to update the position
      transform.position.x = newX;
      transform.position.y = newY;
    }
  }

  void shootable() {
    // Check if the "Space" key is pressed to shoot a bullet
    Uint32 now = SDL_GetTicks();
    Uint32 elapsed = now - lastShootTime;
    if (Game::inputManager.isPressed("Shoot") && elapsed >= 100) {
      auto& transform = entity->getComponent<TransformComponent>();
      auto& bullet(Game::manager.addEntity());
      bullet.addComponent<TransformComponent>(transform.position.x,
                                              transform.position.y, 32, 32, 1);
      bullet.addComponent<BulletComponent>();

      // Retrieve the player's velocity
      auto& playerTransform = entity->getComponent<TransformComponent>();
      float playerVelocityX = playerTransform.velocity.x;
      float playerVelocityY = playerTransform.velocity.y;

      std::cout << "Player velocity: " << playerVelocityX << ", "
                << playerVelocityY << std::endl;

      // Create the bullet entity and get its transform component
      auto& bulletTransform = bullet.getComponent<TransformComponent>();

      // Initialize bullet velocity based on player's velocity
      bulletTransform.velocity.x = playerVelocityX;
      bulletTransform.velocity.y = playerVelocityY;

      // Set bullet velocity based on player direction
      // auto& bulletTransform = bullet.getComponent<TransformComponent>();
      switch (currentDirection) {
        case UP:
          bulletTransform.velocity.y += -1 * bulletSpeed;
          break;
        case DOWN:
          bulletTransform.velocity.y += 1 * bulletSpeed;
          break;
        case LEFT:
          bulletTransform.velocity.x += -1 * bulletSpeed;
          break;
        case RIGHT:
          bulletTransform.velocity.x += 1 * bulletSpeed;
          break;
      }

      // Add the bullet to the enemies group
      bullet.addGroup(Game::groupEnemies);
      lastShootTime = now;
    }
  }

  void draw() override {
    // Drawing logic, if needed
  }

  void handleInput() {
    auto& sprite = entity->getComponent<AnimatedSpriteComponent>();
    auto& transform = entity->getComponent<TransformComponent>();

    sprite.play("Idle");

    // const Uint8* mState = SDL_GetKeyboardState(NULL);
    // std::cout << "got to here" << std::endl;
    // dx = 0;
    // dy = 0;
    transform.velocity = Vector2D(0.0f, 0.0f);

    if (Game::inputManager.isPressed("MoveRight")) {
      // dx = speed;
      sprite.spriteFlip = SDL_FLIP_NONE;
      sprite.playTex("assets/walk-right.png");
      currentDirection = RIGHT;
      transform.velocity.x = speed;
    }
    if (Game::inputManager.isPressed("MoveLeft")) {
      // dx = -speed;
      sprite.spriteFlip = SDL_FLIP_HORIZONTAL;
      sprite.playTex("assets/walk-right.png");
      currentDirection = LEFT;
      transform.velocity.x = -speed;
    }
    if (Game::inputManager.isPressed("MoveUp")) {
      // dy = -speed;
      sprite.spriteFlip = SDL_FLIP_NONE;
      sprite.playTex("assets/walk-up.png");
      currentDirection = UP;
      transform.velocity.y = -speed;
    }
    if (Game::inputManager.isPressed("MoveDown")) {
      // dy = speed;
      sprite.spriteFlip = SDL_FLIP_VERTICAL;
      sprite.playTex("assets/walk-up.png");
      currentDirection = DOWN;
      transform.velocity.y = speed;
    }

    // if (collisionDetected) {
    //   dx = -dx;
    //   dy = -dy;
    // }
  }

 private:
  Uint32 lastShootTime;
};
