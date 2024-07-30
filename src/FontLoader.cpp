#include "FontLoader.hpp"

#include <iostream>

// Define the static member
std::unordered_map<std::string, TTF_Font*> FontLoader::fonts;

void FontLoader::init() {
  if (TTF_Init() == -1) {
    std::cerr << "Failed to initialize SDL_ttf: " << TTF_GetError()
              << std::endl;
  }
}

void FontLoader::quit() {
  clear();
  TTF_Quit();
}

TTF_Font* FontLoader::loadFont(const std::string& filePath, int fontSize) {
  std::string key = filePath + std::to_string(fontSize);
  if (fonts.find(key) == fonts.end()) {
    TTF_Font* font = TTF_OpenFont(filePath.c_str(), fontSize);
    if (font == nullptr) {
      std::cerr << "Failed to load font: " << TTF_GetError() << std::endl;
      return nullptr;
    }
    fonts[key] = font;
  }
  return fonts[key];
}

void FontLoader::clear() {
  for (auto& pair : fonts) {
    TTF_CloseFont(pair.second);
  }
  fonts.clear();
}