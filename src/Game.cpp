#include "Game.h"

#include "ECS/Components.h"
#include "Map.h"
#include "TextureManager.h"
#include "Vector2D.h"

Map* map;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

Manager manager;
auto& player(manager.addEntity());

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

  // Player
  player.addComponent<TransformComponent>();
  player.addComponent<SpriteComponent>("assets/grass.png");
  player.addComponent<KeyboardController>();
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

  // player.getComponent<TransformComponent>().position.Add(Vector2D(5, 0));
  // if (player.getComponent<TransformComponent>().position.x > 500) {
  //   player.getComponent<SpriteComponent>().setTex("assets/dirt.png");
  // }
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