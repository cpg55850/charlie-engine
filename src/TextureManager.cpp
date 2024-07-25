#include "TextureManager.h"

#include "Game.h"

SDL_Texture* TextureManager::LoadTexture(const char* texture) {
  SDL_Surface* tempSurface = IMG_Load(texture);
  SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
  SDL_FreeSurface(tempSurface);

  return tex;
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest,
                          SDL_RendererFlip flip) {
  SDL_Rect outputRect = dest;
  outputRect.x = dest.x - Game::camera.getX() + 450;
  outputRect.y = dest.y - Game::camera.getY() + 250;
  SDL_RenderCopyEx(Game::renderer, tex, &src, &outputRect, NULL, NULL, flip);
}