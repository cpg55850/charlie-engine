#ifndef TEXTUREMANAGER_HPP
#define TEXTUREMANAGER_HPP

#include <SDL.h>

#include <string>
#include <unordered_map>

class TextureManager {
 public:
  // Load a texture from file and cache it
  static SDL_Texture* LoadTexture(const char* texture);
  // Draw a texture to the screen
  static void Draw(SDL_Texture* tex, SDL_Rect src, SDL_Rect dest,
                   SDL_RendererFlip flip);

 private:
  // Cache to store loaded textures
  static std::unordered_map<std::string, SDL_Texture*> textureCache;
};

#endif  // TEXTUREMANAGER_HPP
