#include "scenes/MainMenu.hpp"

#include <iostream>

#include "FontLoader.hpp"
#include "Map.hpp"
#include "scripts/ScriptComponents.hpp"
#include "scripts/PlayerComponent.hpp"
#include "ECS/CameraFollowSystem.hpp"

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
  // ECS systems already ran in SceneManager::update before this call.
  // if (player && player->hasComponent<TransformComponent>()) {
  //   auto& t = player->getComponent<TransformComponent>();
  //   std::cout << "Player pos: (" << t.position.x << ", " << t.position.y << ") Camera: (" << Game::camera.getX() << ", " << Game::camera.getY() << ")" << std::endl;
  // }
}

void MainMenu::draw() {
  // If RenderSystem handles all drawing, this can stay empty or draw UI overlays.
  // Example overlay using label component could be added here.
  // For demonstration, iterate over groups safely:
  // auto& playersGroup = Game::manager.getGroup(Game::groupPlayers);
  // std::cout << "Draw phase: players=" << playersGroup.size() << std::endl;
  // No direct entity->draw calls (RenderSystem already did it). This is just diagnostic.
}
