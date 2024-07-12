#include "GameObject.hpp"

#include <stdio.h>

#include "TextureManager.hpp"

GameObject::GameObject(const char* textureSheet, SDL_Renderer* ren) {
  std::cout << "Creating Game Object" << std::endl;
  renderer = ren;
  objTexture = TextureManager::LoadTexture(textureSheet, ren);
}

void GameObject::Update() {
  xPos = 0;
  yPos = 0;

  srcRect.h = 64 * 9;
  srcRect.w = 64 * 9;
  srcRect.x = 0;
  srcRect.y = 0;

  destRect.x = xPos;
  destRect.y = yPos;
  destRect.w = srcRect.w;
  destRect.h = srcRect.h;
}

void GameObject::Render() {
  SDL_RenderCopy(renderer, objTexture, &srcRect, &destRect);
}