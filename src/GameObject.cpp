#include "GameObject.h"

#include <stdio.h>

#include "TextureManager.h"

GameObject::GameObject(const char* textureSheet, int x, int y) {
  objTexture = TextureManager::LoadTexture(textureSheet);
  xPos = x;
  yPos = y;
}

void GameObject::Update() {
  xPos++;
  yPos++;

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
  SDL_RenderCopy(Game::renderer, objTexture, &srcRect, &destRect);
}