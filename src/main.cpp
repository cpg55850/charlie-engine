#include "Game.h"

Game *game = nullptr;

int main(int argc, const char *argv[]) {
  const int FPS = 60;
  const int frameDelay = 1000 / FPS;  // 16.66ms per frame

  Uint32 frameStart;
  int frameTime;

  game = new Game();

  game->init("GameEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
             1920 / 2, 1080 / 2, false);

  while (game->running()) {
    frameStart = SDL_GetTicks();
    game->handleEvents();
    game->update();
    game->render();
    frameTime = SDL_GetTicks() - frameStart;

    if (frameDelay > frameTime) {
      SDL_Delay(frameDelay - frameTime);
    }
  }

  game->clean();

  return 0;
}