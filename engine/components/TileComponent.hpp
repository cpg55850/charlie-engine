#pragma once

#include <stdio.h>

#include "SDL.h"
#include "SpriteComponent.hpp"
#include "TransformComponent.hpp"

class TileComponent : public Component {
 public:
  SDL_Texture* texture;
  SDL_Rect srcRect, destRect;

  TileComponent() = default;

  ~TileComponent() { SDL_DestroyTexture(texture); }

  TileComponent(int srcX, int srcY, int xPos, int yPos, const char* path) {
    // std::cout << "Hi from tile comopnent " << std::endl << path;
    texture = TextureManager::LoadTexture(path);
    srcRect.x = srcX;
    srcRect.y = srcY;
    srcRect.w = srcRect.h = 16;

    destRect.x = xPos;
    destRect.y = yPos;
    destRect.w = destRect.h = 32;
  }

  void draw() override {
    // SDL_Rect outputRect = {destRect.x + Game::camera.getX(),
    //  destRect.y + Game::camera.getY()};

    // destRect.x = destRect.x - Game::camera.getX();
    // destRect.y = destRect.y - Game::camera.getY();

    // std::cout << outputRect.x << " " << outputRect.y << std::endl;

    TextureManager::Draw(texture, srcRect, destRect, SDL_FLIP_NONE);
  }
};