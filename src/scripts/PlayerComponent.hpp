#pragma once

#include <iostream>

#include "../ECS/Animation.hpp"
#include "../ECS/Components.hpp"
#include "../Game.hpp"

class PlayerComponent : public Component {
 public:
  bool wasPressed = false;
  float hSpeed = 0.0f;
  float vSpeed = 0.0f;

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

  void update() override {
    hSpeed = 0.0f;
    vSpeed = 0.0f;
    auto& transform = entity->getComponent<TransformComponent>();
    auto& sprite = entity->getComponent<SpriteComponent>();

    int xAxis = transform.velocity.x;
    int yAxis = transform.velocity.y;

    if (xAxis != 0) {
      sprite.playTex("assets/walk-right.png", "WalkX");
    }

    if (yAxis != 0) {
      sprite.playTex(yAxis < 0 ? "assets/walk-up.png" : "assets/walk-down.png",
                     yAxis < 0 ? "WalkUp" : "WalkDown");
    }

    const Uint8* state = SDL_GetKeyboardState(NULL);
    bool isPressed = state[SDL_SCANCODE_SPACE];

    if (isPressed && !wasPressed) {
      std::cout << "Bullet fired!" << std::endl;
      auto& bullet = Game::manager.addEntity();
      bullet.addGroup(Game::groupLabels::groupEnemies);
      bullet.addComponent<BulletComponent>();
      auto& bulletTransform = bullet.getComponent<TransformComponent>();
      bulletTransform.setPosition(transform.position.x, transform.position.y);
      // Game::audioManager.playAudio("laser", 0, -1);
    }

    wasPressed = isPressed;

    bool hitSomething = false;

    for (auto cc : Game::colliders) {
      if (Collision::AABB(entity->getComponent<ColliderComponent>(), *cc) &&
          cc->tag == "wall") {
        hitSomething = true;
        transform.velocity.x = 0.0f;
        transform.velocity.y = 0.0f;
        // resolveCollision(transform,
        //                  entity->getComponent<ColliderComponent>().collider,
        //                  cc->collider);
      }
    }

    if (!hitSomething) {
      movePlayer();
    }
  }

  void draw() override {
    // Drawing logic, if needed
  }

  void movePlayer() {
    auto& transform = entity->getComponent<TransformComponent>();
    auto& sprite = entity->getComponent<SpriteComponent>();

    sprite.play("Idle");

    const Uint8* mState = SDL_GetKeyboardState(NULL);

    if (mState[SDL_SCANCODE_D] || mState[SDL_SCANCODE_RIGHT]) {
      hSpeed += 10.0f;
      // sprite.play("Walk");
      sprite.spriteFlip = SDL_FLIP_NONE;
    }
    if (mState[SDL_SCANCODE_A] || mState[SDL_SCANCODE_LEFT]) {
      hSpeed -= 10.0f;
      // sprite.play("Walk");
      sprite.spriteFlip = SDL_FLIP_HORIZONTAL;
    }
    // up/down
    if (mState[SDL_SCANCODE_S] || mState[SDL_SCANCODE_DOWN]) {
      vSpeed += 10.0f;
      // sprite.play("Walk");
      sprite.spriteFlip = SDL_FLIP_HORIZONTAL;
    }
    if (mState[SDL_SCANCODE_W] || mState[SDL_SCANCODE_UP]) {
      vSpeed -= 10.0f;
      // sprite.play("Walk");
      sprite.spriteFlip = SDL_FLIP_NONE;
    }

    transform.velocity.x = hSpeed;
    transform.velocity.y = vSpeed;
  }

 private:
  void resolveCollision(TransformComponent& transform,
                        const SDL_Rect& playerCollider,
                        const SDL_Rect& wallCollider) {
    int deltaX = (playerCollider.x + playerCollider.w / 2) -
                 (wallCollider.x + wallCollider.w / 2);
    int deltaY = (playerCollider.y + playerCollider.h / 2) -
                 (wallCollider.y + wallCollider.h / 2);

    if (abs(deltaX) > abs(deltaY)) {
      if (deltaX > 0) {
        transform.position.x = wallCollider.x + wallCollider.w;
      } else {
        transform.position.x = wallCollider.x - playerCollider.w;
      }
      transform.velocity.x = 0;
    } else {
      if (deltaY > 0) {
        transform.position.y = wallCollider.y + wallCollider.h;
      } else {
        transform.position.y = wallCollider.y - playerCollider.h;
      }
      transform.velocity.y = 0;
    }
  }
};
