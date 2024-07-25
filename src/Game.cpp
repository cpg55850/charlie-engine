#include "Game.h"

#include "Camera.h"
#include "Collision.h"
#include "ECS/Animation.h"
#include "ECS/Components.h"
#include "Map.h"
#include "TextureManager.h"
#include "Vector2D.h"
#include "scripts/ScriptComponents.h"

Map* map;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

Camera Game::camera = Camera(0, 0, 1920, 1080, 2000, 2000);

Manager manager;
auto& player(manager.addEntity());
auto& wall(manager.addEntity());
auto& bullet(manager.addEntity());

const char* mapfile = "assets/tiles.png";

enum groupLabels : std::size_t {
  groupMap,
  groupPlayers,
  groupEnemies,
  groupColliders
};

auto& tiles(manager.getGroup(groupMap));
auto& players(manager.getGroup(groupPlayers));
auto& enemies(manager.getGroup(groupEnemies));

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

      isRunning = true;
    }
  } else {
    isRunning = false;
  }

  // map = new Map();

  const Uint8* state = SDL_GetKeyboardState(NULL);
  // std::cout << "state is" << state << std::endl;
  if (state[SDL_SCANCODE_ESCAPE]) {
    isRunning = false;
  }

  Map::LoadMap("assets/WHAT.csv", 4, 4);

  player.addGroup(groupPlayers);
  wall.addGroup(groupMap);
  bullet.addGroup(groupEnemies);

  player.addComponent<PlayerComponent>();
  bullet.addComponent<BulletComponent>();

  for (auto& p : players) {
    p->init();
  }
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

  camera.update(player.getComponent<TransformComponent>().position.x,
                player.getComponent<TransformComponent>().position.y, 16, 16);

  for (auto cc : colliders) {
    Collision::AABB(player.getComponent<ColliderComponent>(), *cc);
  }
}

void Game::render() {
  SDL_RenderClear(renderer);

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