#include "Game.h"

#include "Camera.h"
#include "Collision.h"
#include "ECS/Components.h"
#include "Map.h"
#include "TextureManager.h"
#include "Vector2D.h"
#include "ECS/Animation.h"

Map* map;

SDL_Renderer* Game::renderer = nullptr;
SDL_Event Game::event;

Camera Game::camera = Camera(0, 0, 1920, 1080, 2000, 2000);

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
  // player.addComponent<SpriteComponent>("assets/walk-right.png", true);
  // "assets/walk-right.png", true, Animation(0, 2, 100), "Walk"
  player.addComponent<SpriteComponent>();
  player.getComponent<SpriteComponent>().addTex("assets/walk-right.png", true, Animation(0, 2, 100), "WalkX");
  player.getComponent<SpriteComponent>().addTex("assets/walk-up.png", true, Animation(0, 2, 100), "WalkUp");
  player.getComponent<SpriteComponent>().addTex("assets/walk-down.png", true, Animation(0, 2, 100), "WalkDown");
  player.getComponent<SpriteComponent>().playTex("assets/walk-right.png", "WalkX");

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

  camera.update(player.getComponent<TransformComponent>().position.x,
                player.getComponent<TransformComponent>().position.y, 16, 16);

  // Player Update
  int xAxis = player.getComponent<TransformComponent>().velocity.x;
  int yAxis = player.getComponent<TransformComponent>().velocity.y;

  if(xAxis > 0) {
    std::cout << "Going right" << std::endl;
    player.getComponent<SpriteComponent>().playTex("assets/walk-right.png", "WalkX");
  } else if (xAxis < 0) {
    std::cout << "Going left" << std::endl;
    player.getComponent<SpriteComponent>().playTex("assets/walk-right.png", "WalkX");
  };

  if(yAxis < 0) {
    std::cout << "Going up" << std::endl;
    player.getComponent<SpriteComponent>().playTex("assets/walk-up.png", "WalkUp");
  } else if (yAxis > 0) {
    std::cout << "Going down" << std::endl;
    player.getComponent<SpriteComponent>().playTex("assets/walk-down.png", "WalkDown");
  };

  // Vector2D pVel = player.getComponent<TransformComponent>().velocity;
  // int pSpeed = player.getComponent<TransformComponent>().speed;

  for (auto cc : colliders) {
    Collision::AABB(player.getComponent<ColliderComponent>(), *cc);
  }
}

void Game::render() {
  SDL_RenderClear(renderer);

  // camera.render(player.getComponent<TransformComponent>().position.x,
  // player.getComponent<TransformComponent>().position.y);

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