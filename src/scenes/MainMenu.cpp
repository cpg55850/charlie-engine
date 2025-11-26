#include "scenes/MainMenu.hpp"

#include <iostream>

#include "../../engine/SDLCore/FontLoader.hpp"
#include "Map.hpp"
#include "../../include/components/ScriptComponents.hpp"
#include "../../include/components/PlayerComponent.hpp"
#include "../../engine/systems/CameraFollowSystem.hpp"
#include "../../include/components/PlayerIdComponent.hpp"

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

  enemy = createEntity();
  enemy->addGroup(Game::groupEnemies);
  enemy->addComponent<EnemyComponent>();

  wall = createEntity();
  wall->addGroup(Game::groupMap);
  // wall->addComponent<WallComponent>(); // Uncomment when implemented

  button = createEntity();
  button->addGroup(Game::groupUI);
  // button->addComponent<ButtonComponent>(); // Uncomment when implemented

  label = createEntity();
  label->addGroup(Game::groupUI);
  // Create a LabelComponent for HUD health
  if (font) {
    auto& hud = *label;
    hud.addComponent<TransformComponent>(10, 10, 0, 0, 1); // dummy transform for label position
    auto& lbl = hud.addComponent<LabelComponent>();
    SDL_Color white{255,255,255,255};
    lbl.setup(font, "Health: 10", white, &hud.getComponent<TransformComponent>());
  }

  // Projectiles spawned dynamically by CombatSystem
}

void MainMenu::onExit() {
  std::cout << "MainMenu::onExit" << std::endl;
  // No manual destroy needed; Scene base destructor handles ownedEntities
}

void MainMenu::update(float deltaTime) {
  if (player && player->hasComponent<TransformComponent>()) {
    auto& t = player->getComponent<TransformComponent>();
    // std::cout << "P1 pos: (" << t.position.x << ", " << t.position.y << ") Camera: (" << Game::camera.getX() << ", " << Game::camera.getY() << ")" << std::endl;
  }
  if (player2 && player2->hasComponent<TransformComponent>()) {
    auto& t2 = player2->getComponent<TransformComponent>();
    // std::cout << "P2 pos: (" << t2.position.x << ", " << t2.position.y << ")" << std::endl;
  }
  if (enemy && enemy->hasComponent<TransformComponent>()) {
    auto& et = enemy->getComponent<TransformComponent>();
    // std::cout << "Enemy pos: (" << et.position.x << ", " << et.position.y << ") vel:(" << et.velocity.x << "," << et.velocity.y << ")" << std::endl;
  }
  // Update HUD label with player health
  // Update HUD label with player health only when it changes to avoid expensive TTF updates
  if (label && label->hasComponent<LabelComponent>() && player && player->hasComponent<PlayerComponent>()) {
    auto& pc = player->getComponent<PlayerComponent>();
    auto& lbl = label->getComponent<LabelComponent>();
    auto& t = label->getComponent<TransformComponent>();
    int lastHealth = t.width; // reuse transform.width as small storage for lastHealth (cheap)
    if (lastHealth != pc.health) {
      lbl.setText("Health: " + std::to_string(pc.health));
      t.width = pc.health; // store new lastHealth
    }
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
