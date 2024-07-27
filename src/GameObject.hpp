#pragma once
#include "Game.hpp"

class GameObject {
 public:
  GameObject(const char* textureSheet, int xPos, int yPos);
  ~GameObject();

  void Update();
  void Render();

 private:
  int xPos;
  int yPos;
  SDL_Texture* objTexture;
  SDL_Rect srcRect, destRect;
};