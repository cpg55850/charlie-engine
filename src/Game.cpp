#include "Game.hpp"
#include "TextureManager.hpp"

SDL_Texture* playerTex;
SDL_Rect srcR, destR;

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

  playerTex = TextureManager::LoadTexture("assets/frog-standing-down-big.png", renderer);
  // SDL_Surface* tmpSurface = IMG_Load("assets/frog-standing-down-big.png");
  // playerTex = SDL_CreateTextureFromSurface(renderer, tmpSurface);
  // SDL_FreeSurface(tmpSurface);
}

void Game::handleEvents() {
  SDL_Event event;
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
  count++;
  destR.h = 64 * 8;
  destR.w = 64 * 8;
  destR.x = sin(count) * 50;
  // destR.y = sin(count) * 100;
  // std::cout << count << std::endl;
}

void Game::render() {
  SDL_RenderClear(renderer);

  // This is where we would add stuff to render
  SDL_RenderCopy(renderer, playerTex, NULL, &destR);

  SDL_RenderPresent(renderer);
}

void Game::clean() {
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
  std::cout << "Game cleaned" << std::endl;
}