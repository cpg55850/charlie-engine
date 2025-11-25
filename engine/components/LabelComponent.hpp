#ifndef LABEL_COMPONENT_HPP
#define LABEL_COMPONENT_HPP

#include <SDL.h>
#include <SDL_ttf.h>

#include <string>

#include "../../include/Game.hpp"
#include "../../include/scripts/Components.hpp"

class LabelComponent : public Component {
 public:
  LabelComponent() = default;
  void setup(TTF_Font* font, const std::string& text, SDL_Color color,
             TransformComponent* transform) {
    this->font = font;
    this->text = text;
    this->color = color;
    this->transform = transform;
    this->texture = nullptr;
    updateTexture();
  }

  ~LabelComponent() {
    if (texture) {
      SDL_DestroyTexture(texture);
    }
  }

  void draw() override {
    if (texture) {
      SDL_RenderCopy(Game::renderer, texture, nullptr, &textRect);
    }
  }

  void setText(const std::string& text) {
    this->text = text;
    updateTexture();
  }

 private:
  TTF_Font* font;
  std::string text;
  SDL_Color color;
  TransformComponent* transform;  // Use TransformComponent to store x and y
  SDL_Texture* texture = nullptr;
  SDL_Rect textRect;

  void updateTexture() {
    if (texture) {
      SDL_DestroyTexture(texture);
    }

    SDL_Surface* textSurface = TTF_RenderText_Solid(font, text.c_str(), color);
    if (textSurface) {
      texture = SDL_CreateTextureFromSurface(
          Game::renderer, textSurface);  // Replace renderer with Game::renderer
      textRect = {static_cast<int>(transform->position.x),
                  static_cast<int>(transform->position.y), textSurface->w,
                  textSurface->h};
      SDL_FreeSurface(textSurface);
    }
  }
};

#endif  // LABEL_COMPONENT_HPP