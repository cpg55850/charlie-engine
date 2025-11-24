#include "Game.hpp"

Game *game = nullptr;

int main(int argc, const char *argv[]) {
  const int FPS = 60;
  const Uint32 frameDelay = 1000 / FPS;  // ms per frame

  Uint32 lastTick = SDL_GetTicks();
  Uint32 frameTime;
  float deltaTime;

  game = new Game();

  game->init("GameEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
             1920 / 2, 1080 / 2, false);

  while (game->running()) {
    Uint32 currentTick = SDL_GetTicks();
    deltaTime = (currentTick - lastTick) / 1000.0f;
    lastTick = currentTick;

    game->handleEvents();
    game->update(deltaTime);
    game->render();

    frameTime = SDL_GetTicks() - currentTick;
    if (frameDelay > frameTime) {
      SDL_Delay(frameDelay - frameTime);
    }
  }

  game->clean();
  delete game;
  game = nullptr;

  return 0;
}
