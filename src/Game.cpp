#include "Game.h"

#include "ECS/Components.h"
#include "Map.h"
#include "TextureManager.h"
#include "Vector2D.h"
#include "Collision.h"

Map* map;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

Manager manager;
auto& player(manager.addEntity());
auto& wall(manager.addEntity());

Game::Game() {}
Game::~Game() {}

void Game::init(const char* title, int xpos, int ypos, int width, int height,
                bool fullscreen) {
  int flags = 0;

  if (fullscreen) {
    flags = SDL_WINDOW_FULLSCREEN;
  }

  if (SDL_Init(SDL_INIT_EVERYTHING) == 0) {
    std::cout << "Subsystems initialised!..." << std::endl;

    window = SDL_CreateWindow(title, xpos, ypos, width, height, flags);
    if (window) {
      std::cout << "Window created!" << std::endl;
    }

    renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer) {
      SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
      std::cout << "Renderer created!" << std::endl;

      isRunning = true;
    }
  } else {
    isRunning = false;
  }

  map = new Map();

  const Uint8* state = SDL_GetKeyboardState(NULL);
  std::cout << "state is" << state << std::endl;
  if (state[SDL_SCANCODE_ESCAPE]) {
    isRunning = false;
  }

  // Player
  player.addComponent<TransformComponent>(2);
  player.addComponent<SpriteComponent>("assets/grass.png");
  player.addComponent<KeyboardController>(state);
  player.addComponent<ColliderComponent>("player");

  wall.addComponent<TransformComponent>(200.0f, 100.0f, 300, 20, 1);
  wall.addComponent<SpriteComponent>("assets/dirt.png");
  wall.addComponent<ColliderComponent>("wall");
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
void Game::update() {
  manager.refresh();
  manager.update();

  if(Collision::AABB(player.getComponent<ColliderComponent>().collider, wall.getComponent<ColliderComponent>().collider)) {
    std::cout << "Wall hit!" << std::endl;
  }
}

void Game::render() {
  SDL_RenderClear(renderer);

  map->DrawMap();
  manager.draw();

  SDL_RenderPresent(renderer);
}

void Game::clean() {
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
  std::cout << "Game cleaned" << std::endl;
}