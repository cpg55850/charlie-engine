#include "scenes/MainMenu.hpp"

#include <iostream>

#include "FontLoader.hpp"
#include "Map.hpp"
#include "scripts/ScriptComponents.hpp"
#include "scripts/PlayerComponent.hpp"
#include "ECS/CameraFollowSystem.hpp"
#include "ECS/PlayerIdComponent.hpp"

class CameraFollowComponent;
MainMenu::MainMenu() : Scene("MainMenu") {}
MainMenu::~MainMenu() {}

void MainMenu::onEnter() {
  std::cout << "MainMenu::onEnter" << std::endl;
  font = FontLoader::loadFont("assets/fonts/zig.ttf", 24);

  Map::LoadMap("assets/WHAT.csv", 4, 4);

  // Create entities
  player = createEntity();
  player->addGroup(Game::groupPlayers);
  player->addComponent<PlayerComponent>(); // auto-wires needed components in PlayerComponent::init
  player->addComponent<CameraFollowComponent>(0, 0); // center player (assuming 1920x1080 window)
  player->addComponent<PlayerIdComponent>(0);
  // Override default transform position for first player
  if (player->hasComponent<TransformComponent>()) {
      auto& t = player->getComponent<TransformComponent>();
      t.position.x = 400; t.position.y = 400;
  }
  // Bind inputs for player1 (WASD + Space)
  if (player->hasComponent<InputComponent>()) {
      auto& inp = player->getComponent<InputComponent>();
      inp.pressedInputs["MoveUp"] = false;
      inp.pressedInputs["MoveDown"] = false;
      inp.pressedInputs["MoveLeft"] = false;
      inp.pressedInputs["MoveRight"] = false;
      inp.pressedInputs["Shoot"] = false;
  }

  // Second player
  player2 = createEntity();
  player2->addGroup(Game::groupPlayers);
  player2->addComponent<PlayerComponent>();
  player2->addComponent<PlayerIdComponent>(1);
  // No camera follow on second to keep primary focus
  if (player2->hasComponent<TransformComponent>()) {
      auto& t2 = player2->getComponent<TransformComponent>();
      t2.position.x = 800; t2.position.y = 600; // different spawn
  }
  if (player2->hasComponent<InputComponent>()) {
      auto& inp2 = player2->getComponent<InputComponent>();
      // Set up alternative keys (expect InputSystem to map these later)
      inp2.pressedInputs["P2_MoveUp"] = false;
      inp2.pressedInputs["P2_MoveDown"] = false;
      inp2.pressedInputs["P2_MoveLeft"] = false;
      inp2.pressedInputs["P2_MoveRight"] = false;
      inp2.pressedInputs["P2_Shoot"] = false;
  }

  enemy = createEntity();
  enemy->addGroup(Game::groupEnemies);
  // enemy->addComponent<EnemyComponent>(); // Uncomment when implemented

  wall = createEntity();
  wall->addGroup(Game::groupMap);
  // wall->addComponent<WallComponent>(); // Uncomment when implemented

  button = createEntity();
  button->addGroup(Game::groupUI);
  // button->addComponent<ButtonComponent>(); // Uncomment when implemented

  label = createEntity();
  label->addGroup(Game::groupUI);
  // label->addComponent<LabelComponent>(); // Uncomment when implemented

  // Projectiles spawned dynamically by CombatSystem
}

void MainMenu::onExit() {
  std::cout << "MainMenu::onExit" << std::endl;
  // No manual destroy needed; Scene base destructor handles ownedEntities
}

void MainMenu::update(float deltaTime) {
  if (player && player->hasComponent<TransformComponent>()) {
    auto& t = player->getComponent<TransformComponent>();
    std::cout << "P1 pos: (" << t.position.x << ", " << t.position.y << ") Camera: (" << Game::camera.getX() << ", " << Game::camera.getY() << ")" << std::endl;
  }
  if (player2 && player2->hasComponent<TransformComponent>()) {
    auto& t2 = player2->getComponent<TransformComponent>();
    std::cout << "P2 pos: (" << t2.position.x << ", " << t2.position.y << ")" << std::endl;
  }
}

void MainMenu::draw() {
  // If RenderSystem handles all drawing, this can stay empty or draw UI overlays.
  // Example overlay using label component could be added here.
  // For demonstration, iterate over groups safely:
  // auto& playersGroup = Game::manager.getGroup(Game::groupPlayers);
  // std::cout << "Draw phase: players=" << playersGroup.size() << std::endl;
  // No direct entity->draw calls (RenderSystem already did it). This is just diagnostic.
}
