#include "MainMenu.hpp"

#include <iostream>

#include "../Collision.hpp"
#include "../Map.hpp"
#include "../scripts/ScriptComponents.hpp"

auto& player(Game::manager.addEntity());
auto& bullet(Game::manager.addEntity());
auto& bullet2(Game::manager.addEntity());
auto& wall(Game::manager.addEntity());

auto& tiles(Game::manager.getGroup(Game::groupMap));
auto& players(Game::manager.getGroup(Game::groupPlayers));
auto& enemies(Game::manager.getGroup(Game::groupEnemies));

// Constructor now initializes the Scene base class with a name
MainMenu::MainMenu() : Scene("MainMenu") {
  // Constructor body: Additional initialization if needed
}

MainMenu::~MainMenu() {
  // Destructor implementation
}

void MainMenu::onEnter() {
  std::cout << "Hello from Main Menu scene" << std::endl;

  // const Uint8* state = SDL_GetKeyboardState(NULL);
  // std::cout << "state is" << state << std::endl;
  // if (state[SDL_SCANCODE_ESCAPE]) {
  //   Game::isRunning = false;
  // }

  Map::LoadMap("assets/WHAT.csv", 4, 4);
  // Game::audioManager.loadAudio("assets/laserShoot.wav", "laser");

  player.addGroup(Game::groupPlayers);
  wall.addGroup(Game::groupMap);
  bullet.addGroup(Game::groupEnemies);
  bullet2.addGroup(Game::groupEnemies);

  player.addComponent<PlayerComponent>();
  // bullet.addComponent<BulletComponent>();
  bullet2.addComponent<BulletComponent>();
  wall.addComponent<WallComponent>();

  // Load textures, create entities, set up components, etc.
  // Example:
  // auto& titleEntity = Game::manager.addEntity();
  // titleEntity.addComponent<TransformComponent>(/*...*/);
  // titleEntity.addComponent<SpriteComponent>();
  // titleEntity.getComponent<SpriteComponent>().addTex(
  //     "assets/main_menu.png", true, Animation(0, 1, 100), "MainMenuTitle");

  // Add more entities and components as needed
}

void MainMenu::onExit() {
  // std::cout << "Unloading MainMenu scene" << std::endl;

  // Clean up resources
  // Game::manager.clean();  // Optionally clean up entities
}

void MainMenu::update() {
  // Update entities, handle input, etc.
  // Game::manager.update();
  // std::cout << "updating MainMenu scene" << std::endl;

  Game::camera.update(player.getComponent<TransformComponent>().position.x,
                      player.getComponent<TransformComponent>().position.y, 16,
                      16);

  // for (auto cc : Game::colliders) {
  //   Collision::AABB(player.getComponent<ColliderComponent>(), *cc);
  // }
  // Example: Transition to another scene if a condition is met
  // if (/* condition */) {
  //     // Transition to the next scene
  //     Game::sceneManager.switchScene("NextScene");
  // }
}

void MainMenu::draw() {
  // Render entities and other scene elements
  // Game::manager.render();
  // std::cout << "Draw the scene" << std::endl;

  for (auto& t : tiles) {
    t->draw();
  }
  for (auto& p : players) {
    p->draw();
  }
  for (auto& e : enemies) {
    e->draw();
  }
}
