#include "TextureManager.hpp"

#include <SDL_image.h>

#include <iostream>

#include "Game.hpp"

// Initialize the texture cache
std::unordered_map<std::string, SDL_Texture*> TextureManager::textureCache;

SDL_Texture* TextureManager::LoadTexture(const char* texture) {
  std::string texturePath(texture);

  // Check if the texture is already loaded
  auto it = textureCache.find(texturePath);
  if (it != textureCache.end()) {
    return it->second;  // Return the cached texture
  }

  // Load raw image data into RAM
  SDL_Surface* tempSurface = IMG_Load(texture);
  if (!tempSurface) {
    std::cerr << "Unable to load image: " << IMG_GetError() << std::endl;
    return nullptr;
  }

  // Create GPU texture from image data
  SDL_Texture* tex = SDL_CreateTextureFromSurface(Game::renderer, tempSurface);
  SDL_FreeSurface(tempSurface);

  if (!tex) {
    std::cerr << "Unable to create texture: " << SDL_GetError() << std::endl;
    return nullptr;
  }

  // Cache the loaded texture
  textureCache[texturePath] = tex;

  return tex;
}

void TextureManager::Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest,
                          SDL_RendererFlip flip) {
  SDL_Rect outputRect = dest;
  outputRect.x = dest.x - Game::camera.getX() + 450;
  outputRect.y = dest.y - Game::camera.getY() + 250;
  SDL_RenderCopyEx(Game::renderer, tex, &src, &outputRect, 0.0, NULL, flip);
}
