#include "GameObject.hpp"

#include "TextureManager.hpp"

GameObject::GameObject(const char* textureSheet, int x, int y) {
  objTexture = TextureManager::LoadTexture(textureSheet);
  xPos = x;
  yPos = y;
}

void GameObject::Update() {
  // xPos++;
  // yPos++;

  srcRect.h = 64 * 12;
  srcRect.w = 64 * 12;
  srcRect.x = 0;
  srcRect.y = 0;

  destRect.x = xPos;
  destRect.y = yPos;
  destRect.w = srcRect.w / 12;
  destRect.h = srcRect.h / 12;
}

void GameObject::Render() {
  SDL_RenderCopy(Game::renderer, objTexture, &srcRect, &destRect);
}