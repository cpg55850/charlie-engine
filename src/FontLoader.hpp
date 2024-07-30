#ifndef FONTLOADER_HPP
#define FONTLOADER_HPP

#include <SDL_ttf.h>

#include <string>
#include <unordered_map>

class FontLoader {
 public:
  static void init();
  static void quit();
  static TTF_Font* loadFont(const std::string& filePath, int fontSize);
  static void clear();

 private:
  static std::unordered_map<std::string, TTF_Font*> fonts;
};

#endif  // FONTLOADER_HPP