#include "Game.hpp"

#include "Camera.hpp"
#include "Collision.hpp"
#include "ECS/Components.hpp"
#include "ECS/InputSystem.hpp"
#include "ECS/ScriptSystem.hpp"
#include "ECS/CombatSystem.hpp"
#include "ECS/CollisionSystem.hpp"
#include "ECS/MovementSystem.hpp"
#include "ECS/LifetimeSystem.hpp"
#include "ECS/AnimationSystem.hpp"
#include "ECS/CameraFollowSystem.hpp"
#include "ECS/RenderSystem.hpp"
#include "FontLoader.hpp"
#include "Map.hpp"
#include "SceneFactory.hpp"
#include "ECS/EnemyAISystem.hpp"
#include "ECS/PlayerInputSystem.hpp"
#include "scenes/MainMenu.hpp"
#include "scripts/ScriptComponents.hpp"

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

Camera Game::camera(0, 0, 1920, 1080, 2000, 2000);

Manager Game::manager;
AudioManager Game::audioManager;

std::vector<ColliderComponent*> Game::colliders;

bool Game::isRunning = true;
const char* mapfile = "assets/tiles.png";

Game::Game() {}
Game::~Game() {}

void Game::init(const char* title, int xpos, int ypos, int width, int height,
                bool fullscreen)
{
    int flags = SDL_WINDOW_RESIZABLE;
    if (fullscreen) {
        flags = SDL_WINDOW_FULLSCREEN;
    }

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cerr << "SDL initialization failed." << std::endl;
        isRunning = false;
        return;
    }

    std::cout << "Subsystems initialized!" << std::endl;

    window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    renderer = SDL_CreateRenderer(window, -1,
                                  SDL_RENDERER_ACCELERATED |
                                  SDL_RENDERER_PRESENTVSYNC);

    if (!renderer) {
        std::cerr << "Renderer creation failed." << std::endl;
        isRunning = false;
        return;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);

    // Load audio, fonts, scenes
    audioManager.loadAudio("assets/laserShoot.wav", "laser");
    FontLoader::init();

    SceneFactory::instance().registerScene(
        "MainMenu", []() { return std::make_unique<MainMenu>(); });

    // Register ECS systems (guard against duplicate adds on re-init) in proper order
    if (!manager.hasSystem<InputSystem>()) manager.addSystem<InputSystem>();            // 1. Poll raw input
    if (!manager.hasSystem<PlayerInputSystem>()) manager.addSystem<PlayerInputSystem>(); // 2. Translate input to movement/combat intent
    if (!manager.hasSystem<ScriptSystem>()) manager.addSystem<ScriptSystem>();          // (Optional legacy scripts)
    if (!manager.hasSystem<CombatSystem>()) manager.addSystem<CombatSystem>();          // 3. Spawn projectiles from requests
    if (!manager.hasSystem<EnemyAISystem>()) manager.addSystem<EnemyAISystem>();      // AI sets velocities
    if (!manager.hasSystem<MovementSystem>()) manager.addSystem<MovementSystem>();      // 4. Apply velocity
    if (!manager.hasSystem<CollisionSystem>()) manager.addSystem<CollisionSystem>();    // 5. Resolve collisions after movement
    if (!manager.hasSystem<LifetimeSystem>()) manager.addSystem<LifetimeSystem>();      // 6. Cleanup timed entities
    if (!manager.hasSystem<AnimationSystem>()) manager.addSystem<AnimationSystem>();    // 7. Animate sprites
    if (!manager.hasSystem<CameraFollowSystem>()) manager.addSystem<CameraFollowSystem>(); // 8. Update camera
    if (!manager.hasSystem<RenderSystem>()) manager.addSystem<RenderSystem>();          // 9. Draw everything

    // Load initial scene
    sceneManager.switchScene("MainMenu");
}

void Game::handleEvents() {
  SDL_PollEvent(&event);
  if (event.type == SDL_QUIT) {
    isRunning = false;
  }
}

void Game::update(float deltaTime) {
  sceneManager.update(deltaTime);  // Update scene logic
  manager.refresh();               // Cleanup ECS entities/groups
}

void Game::render() {
  SDL_RenderClear(renderer);
  // Run render system now (draw happens after clear)
  try {
    auto &renderSystem = manager.getSystem<RenderSystem>();
    renderSystem.update(manager, 0.f); // deltaTime not needed for drawing
  } catch (const std::exception &e) {
    // RenderSystem might not be registered yet
  }
  // Scene-specific overlays (UI) can still draw
  sceneManager.draw();
  SDL_RenderPresent(renderer);
}

void Game::clean() {
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  FontLoader::quit();
  SDL_Quit();
  std::cout << "Game cleaned" << std::endl;
}

void Game::AddTile(int srcX, int srcY, int xPos, int yPos) {
  auto& tile(manager.addEntity());
  tile.addComponent<TileComponent>(srcX, srcY, xPos, yPos, mapfile);
  tile.addGroup(groupMap);
}