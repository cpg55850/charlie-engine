#include "Game.h"

#include "Collision.h"
#include "ECS/Components.h"
#include "Map.h"
#include "TextureManager.h"
#include "Vector2D.h"

Map* map;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

SDL_Rect Game::camera = {0, 0, 800, 640};

Manager manager;
auto& player(manager.addEntity());
auto& wall(manager.addEntity());

const char* mapfile = "assets/tiles.png";

enum groupLabels : std::size_t {
  groupMap,
  groupPlayers,
  groupEnemies,
  groupColliders
};

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

  // map = new Map();

  const Uint8* state = SDL_GetKeyboardState(NULL);
  std::cout << "state is" << state << std::endl;
  if (state[SDL_SCANCODE_ESCAPE]) {
    isRunning = false;
  }

  // Tiles
  // tile0.addComponent<TileComponent>(200, 200, 32, 32, 0);
  // tile1.addComponent<TileComponent>(250, 250, 32, 32, 1);
  // tile2.addComponent<TileComponent>(150, 150, 32, 32, 2);

  // tile1.addComponent<ColliderComponent>("dirt");
  // tile2.addComponent<ColliderComponent>("grass");

  Map::LoadMap("assets/WHAT.csv", 4, 4);

  // Player
  player.addComponent<TransformComponent>(4);
  // player.addComponent<SpriteComponent>("assets/idle.png", false);
  player.addComponent<SpriteComponent>("assets/walk-right.png", true);

  player.addComponent<KeyboardController>(state);
  player.addComponent<ColliderComponent>("player");
  player.addGroup(groupPlayers);

  // Wall
  // wall.addComponent<TransformComponent>(200.0f, 100.0f, 300, 20, 1);
  // wall.addComponent<SpriteComponent>("assets/dirt.png");
  // wall.addComponent<ColliderComponent>("wall");
  wall.addGroup(groupMap);
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

  camera.x = player.getComponent<TransformComponent>().position.x - 400;
  camera.y = player.getComponent<TransformComponent>().position.x - 320;

  if (camera.x < 0) {
    camera.x = 0;
  }
  if (camera.y < 0) {
    camera.y = 0;
  }
  if (camera.x > camera.w) {
    camera.x = camera.w;
  }
  if (camera.y > camera.h) {
    camera.y = camera.h;
  }

  for (auto cc : colliders) {
    Collision::AABB(player.getComponent<ColliderComponent>(), *cc);
  }
}

auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));

void Game::render() {
  SDL_RenderClear(renderer);

  // manager.draw();
  for (auto& t : tiles) {
    t->draw();
  }
  for (auto& p : players) {
    p->draw();
  }
  for (auto& e : enemies) {
    e->draw();
  }

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