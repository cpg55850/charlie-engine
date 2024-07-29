#include "Game.hpp"

Game *game = nullptr;

int main(int argc, const char *argv[]) {
  const int FPS = 60;
  const int frameDelay = 1000 / FPS;  // 16.66ms per frame

  Uint32 frameStart;
  int frameTime;
  float deltaTime;

  game = new Game();

  game->init("GameEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
             1920 / 2, 1080 / 2, false);

  while (game->running()) {
    frameStart = SDL_GetTicks();
    game->handleEvents();

    // Calculate delta time
    deltaTime = 1 + ((SDL_GetTicks() - frameStart) / 1000.0f);

    game->update(deltaTime);
    game->render();
    frameTime = SDL_GetTicks() - frameStart;

    if (frameDelay > frameTime) {
      SDL_Delay(frameDelay - frameTime);
    }
  }

  game->clean();

  return 0;
}