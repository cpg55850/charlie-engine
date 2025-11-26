#define SDL_MAIN_HANDLED
#include <SDL.h>
#include "Game.hpp"

int main(int argc, const char *argv[]) {
  constexpr int FPS = 60;
  constexpr Uint32 frameDelay = 1000 / FPS;  // ms per frame

  Uint32 lastTick = SDL_GetTicks();

  // Use a smart pointer to manage the Game instance
  const auto game = std::make_unique<Game>();

  game->init("GameEngine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
             1920 / 2, 1080 / 2, false);

  while (game->running()) {
    const Uint32 currentTick = SDL_GetTicks();
    float deltaTime = (currentTick - lastTick) / 1000.0f;
    lastTick = currentTick;

    game->handleEvents();
    game->update(deltaTime);
    game->render();

    Uint32 frameTime = SDL_GetTicks() - currentTick;
    if (frameDelay > frameTime) {
      SDL_Delay(frameDelay - frameTime);
    }
  }

  return 0;
}
