#include "scenes/MainMenu.hpp"

#include <iostream>

#include "Collision.hpp"
#include "FontLoader.hpp"
#include "Map.hpp"
#include "ECS/Systems.hpp"
#include "scripts/ScriptComponents.hpp"

auto& player(Game::manager.addEntity());
auto& bullet(Game::manager.addEntity());
auto& bullet2(Game::manager.addEntity());
auto& wall(Game::manager.addEntity());
auto& enemy(Game::manager.addEntity());
auto& button(Game::manager.addEntity());
auto& label(Game::manager.addEntity());

auto& tiles(Game::manager.getGroup(Game::groupMap));
auto& players(Game::manager.getGroup(Game::groupPlayers));
auto& enemies(Game::manager.getGroup(Game::groupEnemies));
auto& projectiles(Game::manager.getGroup(Game::groupProjectiles));
TTF_Font* font = nullptr;

// Constructor now initializes the Scene base class with a name
MainMenu::MainMenu() : Scene("MainMenu") {
  // Constructor body: Additional initialization if needed
}

MainMenu::~MainMenu() {
  // Destructor implementation
}

void MainMenu::onEnter() {
  std::cout << "Hello from Main Menu scene" << std::endl;

  font = FontLoader::loadFont("assets/fonts/zig.ttf", 24);

  // Initialize ECS Systems in correct order
  // IMPORTANT: Systems must be added for the game to work!
  Game::manager.addSystem<InputSystem>();       // 1. Updates input state
  Game::manager.addSystem<ScriptSystem>();      // 2. Runs PlayerComponent (sets velocity, requests shoot)
  Game::manager.addSystem<CombatSystem>();      // 3. Spawns bullets based on shoot requests
  Game::manager.addSystem<CollisionSystem>();   // 4. Predicts & prevents collisions (modifies velocity)
  Game::manager.addSystem<MovementSystem>();    // 5. Applies velocity to position
  Game::manager.addSystem<LifetimeSystem>();    // 6. Destroys old/off-screen entities
  Game::manager.addSystem<AnimationSystem>();   // 7. Updates animations based on state
  Game::manager.addSystem<RenderSystem>();      // 8. Renders everything (must be last!)

  Map::LoadMap("assets/WHAT.csv", 4, 4);
  // Game::audioManager.loadAudio("assets/laserShoot.wav", "laser");

  player.addGroup(Game::groupPlayers);
  // wall.addGroup(Game::groupMap);
  // bullet.addGroup(Game::groupEnemies);
  // bullet2.addGroup(Game::groupEnemies);
  // enemy.addGroup(Game::groupEnemies);
  // label.addGroup(Game::groupPlayers);

  player.addComponent<PlayerComponent>();
  // bullet.addComponent<BulletComponent>();
  // bullet2.addComponent<BulletComponent>();
  // wall.addComponent<WallComponent>();
  // enemy.addComponent<EnemyComponent>();
  // button.addComponent<ButtonComponent>();
  // label.addComponent<LabelComponent>();

  // TransformComponent labelTransform;
  // labelTransform.position.x = 0;
  // labelTransform.position.y = 0;
  // labelTransform.width = 100;
  // labelTransform.height = 100;
  // labelTransform.scale = 1;
  // label.addComponent<LabelComponent>();
  // label.getComponent<LabelComponent>().setup(font, "Hello, World!",
  //                                            {255, 255, 255}, &labelTransform);

  // button.getComponent<ButtonComponent>().setup("assets/water.png", 100, 100,
  //  100, 100, "Play", "Play");

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

void MainMenu::update(float deltaTime) {
  // Update entities, handle input, etc.
  Game::manager.update(deltaTime);
  std::cout << "updating MainMenu scene" << std::endl;

  Game::camera.update(player.getComponent<TransformComponent>().position.x,
                      player.getComponent<TransformComponent>().position.y, 16,
                      16);

  std::cout << "Player position: " << player.getComponent<TransformComponent>() .position.x << ", " << player.getComponent<TransformComponent>().position.y << std::endl;

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
  std::cout << "Draw the scene" << std::endl;

  for (auto& t : tiles) {
    t->draw();
  }
  for (auto& p : players) {
    p->draw();
  }
  for (auto& e : enemies) {
    e->draw();
  }
  for (auto& proj : projectiles) {
    proj->draw();
  }
}
