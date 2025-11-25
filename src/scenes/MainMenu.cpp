#include "scenes/MainMenu.hpp"

#include <iostream>

#include "FontLoader.hpp"
#include "Map.hpp"
#include "ECS/Systems.hpp"
#include "scripts/ScriptComponents.hpp"
#include "Collision.hpp"
#include "ECS/CameraFollowSystem.hpp"

class CameraFollowComponent;
MainMenu::MainMenu() : Scene("MainMenu") {}
MainMenu::~MainMenu() {}

void MainMenu::onEnter() {
  std::cout << "MainMenu::onEnter" << std::endl;
  font = FontLoader::loadFont("assets/fonts/zig.ttf", 24);

  Map::LoadMap("assets/WHAT.csv", 4, 4);

  // Create entities
  player = &Game::manager.addEntity();
  player->addGroup(Game::groupPlayers);
  player->addComponent<PlayerComponent>();
  player->addComponent<CameraFollowComponent>(0, 0); // center player (assuming 1920x1080 window)

  enemy = &Game::manager.addEntity();
  enemy->addGroup(Game::groupEnemies);
  // enemy->addComponent<EnemyComponent>(); // Uncomment when implemented

  wall = &Game::manager.addEntity();
  wall->addGroup(Game::groupMap);
  // wall->addComponent<WallComponent>(); // Uncomment when implemented

  button = &Game::manager.addEntity();
  button->addGroup(Game::groupUI);
  // button->addComponent<ButtonComponent>(); // Uncomment when implemented

  label = &Game::manager.addEntity();
  label->addGroup(Game::groupUI);
  // label->addComponent<LabelComponent>(); // Uncomment when implemented

  // Projectiles spawned dynamically by CombatSystem
}

void MainMenu::onExit() {
  std::cout << "MainMenu::onExit" << std::endl;
  // Mark entities inactive (systems/manager will clean them up)
  auto destroyIf = [](Entity* e) { if (e) e->destroy(); };
  destroyIf(player);
  destroyIf(enemy);
  destroyIf(wall);
  destroyIf(button);
  destroyIf(label);
  // font cleanup handled by FontLoader globally
}

void MainMenu::update(float deltaTime) {
  // ECS systems already ran in SceneManager::update before this call.
  if (player && player->hasComponent<TransformComponent>()) {
    auto& t = player->getComponent<TransformComponent>();
    std::cout << "Player pos: (" << t.position.x << ", " << t.position.y << ") Camera: (" << Game::camera.getX() << ", " << Game::camera.getY() << ")" << std::endl;
  }
}

void MainMenu::draw() {
  // If RenderSystem handles all drawing, this can stay empty or draw UI overlays.
  // Example overlay using label component could be added here.
  // For demonstration, iterate over groups safely:
  auto& playersGroup = Game::manager.getGroup(Game::groupPlayers);
  std::cout << "Draw phase: players=" << playersGroup.size() << std::endl;
  // No direct entity->draw calls (RenderSystem already did it). This is just diagnostic.
}
