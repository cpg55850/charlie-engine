//
// Created by Charlie Graham on 11/23/25.
//

#include "Collision.hpp"
#include "scripts/PlayerComponent.hpp"

#include <iostream>

#include "ECS/AnimatedSpriteComponent.hpp"
#include "ECS/ColliderComponent.hpp"
#include "scripts/ScriptComponents.hpp"

PlayerComponent::~PlayerComponent() {}

void PlayerComponent::init() {
  std::cout << "PlayerComponent initialized!" << std::endl;
  std::cout << "Entity address: " << entity << std::endl;

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

void PlayerComponent::update(float deltaTime) {
  handleInput();
  shootable();
  movePlayer(deltaTime);

  auto& transform = entity->getComponent<TransformComponent>();
  transform.velocity.x = 0;
  transform.velocity.y = 0;
}

void PlayerComponent::movePlayer(float deltaTime) {
  auto& transform = entity->getComponent<TransformComponent>();

  float newX = transform.position.x + transform.velocity.x * deltaTime;
  float newY = transform.position.y + transform.velocity.y * deltaTime;

  SDL_Rect tempCollider = {static_cast<int>(newX), static_cast<int>(newY),
                           transform.width * transform.scale,
                           transform.height * transform.scale};

  bool collisionDetected = false;
  for (auto cc : Game::colliders) {
    if (cc->tag == "wall" && Collision::AABB(tempCollider, cc->collider)) {
      collisionDetected = true;
      std::cout << "Hit something!" << std::endl;
      break;
    }
  }

  if (collisionDetected) {
    float stepSize = 0.1f;

    while (true) {
      float nextVelocityX = transform.velocity.x * stepSize;
      float nextVelocityY = transform.velocity.y * stepSize;

      float nextX = transform.position.x + nextVelocityX;
      float nextY = transform.position.y + nextVelocityY;

      SDL_Rect nextTempCollider = {static_cast<int>(nextX),
                                   static_cast<int>(nextY),
                                   transform.width * transform.scale,
                                   transform.height * transform.scale};

      bool willCollide = false;
      for (auto cc : Game::colliders) {
        if (cc->tag == "wall" && Collision::AABB(nextTempCollider, cc->collider)) {
          willCollide = true;
          break;
        }
      }

      if (willCollide) {
        break;
      }

      transform.position.x = nextX;
      transform.position.y = nextY;
    }

    transform.velocity.x = 0;
    transform.velocity.y = 0;
  } else {
    transform.position.x = newX;
    transform.position.y = newY;
  }
}

void PlayerComponent::shootable() {
  Uint32 now = SDL_GetTicks();
  Uint32 elapsed = now - lastShootTime;
  if (Game::inputManager.isPressed("Shoot") && elapsed >= 100) {
    auto& transform = entity->getComponent<TransformComponent>();
    auto& bullet(Game::manager.addEntity());
    bullet.addComponent<TransformComponent>(transform.position.x,
                                            transform.position.y, 32, 32, 1);
    bullet.addComponent<BulletComponent>();

    auto& playerTransform = entity->getComponent<TransformComponent>();
    float playerVelocityX = playerTransform.velocity.x;
    float playerVelocityY = playerTransform.velocity.y;

    std::cout << "Player velocity: " << playerVelocityX << ", "
              << playerVelocityY << std::endl;

    auto& bulletTransform = bullet.getComponent<TransformComponent>();

    bulletTransform.velocity.x = playerVelocityX;
    bulletTransform.velocity.y = playerVelocityY;

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

    bullet.addGroup(Game::groupEnemies);
    lastShootTime = now;
  }
}

void PlayerComponent::draw() {
  // nothing here for now
}

void PlayerComponent::handleInput() {
  auto& sprite = entity->getComponent<AnimatedSpriteComponent>();
  auto& transform = entity->getComponent<TransformComponent>();

  sprite.play("Idle");

  transform.velocity = Vector2D(0.0f, 0.0f);

  if (Game::inputManager.isPressed("MoveRight")) {
    sprite.spriteFlip = SDL_FLIP_NONE;
    sprite.playTex("assets/walk-right.png");
    currentDirection = RIGHT;
    transform.velocity.x = speed;
  }
  if (Game::inputManager.isPressed("MoveLeft")) {
    sprite.spriteFlip = SDL_FLIP_HORIZONTAL;
    sprite.playTex("assets/walk-right.png");
    currentDirection = LEFT;
    transform.velocity.x = -speed;
  }
  if (Game::inputManager.isPressed("MoveUp")) {
    sprite.spriteFlip = SDL_FLIP_NONE;
    sprite.playTex("assets/walk-up.png");
    currentDirection = UP;
    transform.velocity.y = -speed;
  }
  if (Game::inputManager.isPressed("MoveDown")) {
    sprite.spriteFlip = SDL_FLIP_VERTICAL;
    sprite.playTex("assets/walk-up.png");
    currentDirection = DOWN;
    transform.velocity.y = speed;
  }
}
