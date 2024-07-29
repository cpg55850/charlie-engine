#include "Game.hpp"

#include "Camera.hpp"
#include "Collision.hpp"
#include "ECS/Animation.hpp"
#include "ECS/Components.hpp"
#include "Map.hpp"
#include "SceneFactory.hpp"
#include "TextureManager.hpp"
#include "Vector2D.hpp"
#include "scenes/MainMenu.hpp"
#include "scripts/ScriptComponents.hpp"

InputManager Game::inputManager;

void setupInputs() {
  Game::inputManager.addInput("MoveRight",
                              {SDL_SCANCODE_D, SDL_SCANCODE_RIGHT});
  Game::inputManager.addInput("MoveLeft", {SDL_SCANCODE_A, SDL_SCANCODE_LEFT});
  Game::inputManager.addInput("MoveUp", {SDL_SCANCODE_W, SDL_SCANCODE_UP});
  Game::inputManager.addInput("MoveDown", {SDL_SCANCODE_S, SDL_SCANCODE_DOWN});
  Game::inputManager.addInput("Shoot", {SDL_SCANCODE_SPACE});
}

Map* map;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

Camera Game::camera = Camera(0, 0, 1920, 1080, 2000, 2000);

Manager Game::manager = Manager();
AudioManager Game::audioManager;

const char* mapfile = "assets/tiles.png";
bool Game::isRunning = true;

// auto& tile0(manager.addEntity());
// auto& tile1(manager.addEntity());
// auto& tile2(manager.addEntity());

std::vector<ColliderComponent*> Game::colliders;

Game::Game() {}
Game::~Game() {}

void Game::init(const char* title, int xpos, int ypos, int width, int height,
                bool fullscreen) {
  int flags = 0;

  // Makes the window resizable and adds maximize support
  flags = SDL_WINDOW_RESIZABLE;

  if (fullscreen) {
    flags = SDL_WINDOW_FULLSCREEN;
  }

  if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
    std::cout << "Subsystems initialised!..." << std::endl;

    window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    // if (window) {
    // std::cout << "Window created!" << std::endl;
    // }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer) {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      // std::cout << "Renderer created!" << std::endl;

      Game::isRunning = true;
    }
  } else {
    Game::isRunning = false;
  }

  setupInputs();

  // if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
  //   std::cerr << "SDL_mixer could not initialize! Mix_Error: " <<
  //   Mix_GetError()
  //             << std::endl;
  //   return;
  // }

  SceneFactory::instance().registerScene(
      "MainMenu", []() { return std::make_unique<MainMenu>(); });

  // Initialize SceneManager and load initial scene
  sceneManager.loadScene("MainMenu");
  sceneManager.switchScene("MainMenu");
  // map = new Map();
}

void Game::handleEvents() {
  SDL_PollEvent(&event);
  switch (event.type) {
    case SDL_QUIT:
      isRunning = false;
      break;
    default:
      break;
  }
}
void Game::update(float deltaTime) {
  // manager.refresh();
  Game::inputManager.update();
  manager.update(deltaTime);
  sceneManager.update(deltaTime);  // Update the current scene
}

void Game::render() {
  SDL_RenderClear(renderer);
  sceneManager.draw();  // Draw the current scene

  // for (auto& t : tiles) {
  //   t->draw();
  // }
  // for (auto& p : players) {
  //   p->draw();
  // }
  // for (auto& e : enemies) {
  //   e->draw();
  // }

  SDL_RenderPresent(renderer);
}

void Game::clean() {
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
  std::cout << "Game cleaned" << std::endl;
}

void Game::AddTile(int srcX, int srcY, int xPos, int yPos) {
  auto& tile(manager.addEntity());
  tile.addComponent<TileComponent>(srcX, srcY, xPos, yPos, mapfile);
  tile.addGroup(groupMap);
}