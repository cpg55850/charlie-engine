//
// Created by Charlie Graham on 11/23/25.
//
// Refactored to work with ECS Systems:
// - InputSystem handles input state
// - MovementSystem updates position based on velocity
// - AnimationSystem updates sprite frames
// - CollisionSystem detects collisions
// - This script only sets velocity and handles game logic

#include "Collision.hpp"
#include "scripts/PlayerComponent.hpp"

#include <iostream>

#include "ECS/AnimatedSpriteComponent.hpp"
#include "ECS/AnimationStateComponent.hpp"
#include "ECS/ColliderComponent.hpp"
#include "ECS/CombatComponent.hpp"
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

  // Add InputComponent for ECS-based input handling
  entity->addComponent<InputComponent>();

  // Add AnimationStateComponent for animation control
  entity->addComponent<AnimationStateComponent>();

  // Add CombatComponent for shooting
  auto& combat = entity->addComponent<CombatComponent>();
  combat.projectileSpeed = 400.0f;
  combat.fireRate = 100.0f;
}

void PlayerComponent::update(float deltaTime) {
  // 1. Handle input - sets velocity based on player input
  handleInput();

  // 2. Handle shooting logic
  shootable();

  // Note: CollisionSystem will handle collision detection and response
  // Note: MovementSystem will apply velocity to position
  // PlayerComponent only handles player-specific game logic!
}

void PlayerComponent::shootable() {
  auto& input = entity->getComponent<InputComponent>();
  auto& combat = entity->getComponent<CombatComponent>();
  auto& animState = entity->getComponent<AnimationStateComponent>();

  // PURE ECS: Only REQUEST a shoot, don't create the bullet!
  // CombatSystem will handle bullet spawning
  if (input.isPressed("Shoot")) {
    combat.requestShoot(animState.facingDirection);
  }
}

void PlayerComponent::draw() {
  // nothing here for now
}

void PlayerComponent::handleInput() {
  auto& transform = entity->getComponent<TransformComponent>();
  auto& input = entity->getComponent<InputComponent>();
  auto& animState = entity->getComponent<AnimationStateComponent>();

  // Reset velocity each frame
  transform.velocity = Vector2D(0.0f, 0.0f);
  animState.isMoving = false;

  // PURE ECS: Only set GAME STATE (velocity, direction, isMoving)
  // AnimationSystem will choose the animation based on this state!

  if (input.isPressed("MoveRight")) {
    animState.facingDirection = AnimationStateComponent::RIGHT;
    animState.isMoving = true;
    transform.velocity.x = speed;
  }
  if (input.isPressed("MoveLeft")) {
    animState.facingDirection = AnimationStateComponent::LEFT;
    animState.isMoving = true;
    transform.velocity.x = -speed;
  }
  if (input.isPressed("MoveUp")) {
    animState.facingDirection = AnimationStateComponent::UP;
    animState.isMoving = true;
    transform.velocity.y = -speed;
  }
  if (input.isPressed("MoveDown")) {
    animState.facingDirection = AnimationStateComponent::DOWN;
    animState.isMoving = true;
    transform.velocity.y = speed;
  }

  // No sprite.playTex() here! AnimationSystem handles that.
}
