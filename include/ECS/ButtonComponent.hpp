#ifndef BUTTONCOMPONENT_HPP
#define BUTTONCOMPONENT_HPP

#include <SDL.h>
#include <SDL_ttf.h>

#include <functional>
#include <string>

// extern SDL_Renderer* renderer;  // Assuming renderer is defined elsewhere

class ButtonComponent : public Component {
 public:
  ButtonComponent() = default;

  void setup(int x, int y, int width, int height, const std::string& text,
             SDL_Color textColor, SDL_Color buttonColor, TTF_Font* font,
             std::function<void()> onClick) {
    buttonRect = {x, y, width, height};
    this->textColor = textColor;
    this->buttonColor = buttonColor;
    this->text = text;
    this->font = font;
    this->onClick = onClick;
    textTexture = nullptr;
  }

  ~ButtonComponent() {
    if (textTexture) {
      SDL_DestroyTexture(textTexture);
    }
  }

  void init() override { createTextTexture(); }

  void update(float deltaTime) override {
    // Handle any updates if necessary
  }

  void draw() override {
    // Render button
    SDL_SetRenderDrawColor(Game::renderer, buttonColor.r, buttonColor.g,
                           buttonColor.b, buttonColor.a);
    SDL_RenderFillRect(Game::renderer, &buttonRect);

    // Render text
    if (textTexture) {
      int textWidth, textHeight;
      SDL_QueryTexture(textTexture, nullptr, nullptr, &textWidth, &textHeight);
      SDL_Rect textRect = {buttonRect.x + (buttonRect.w - textWidth) / 2,
                           buttonRect.y + (buttonRect.h - textHeight) / 2,
                           textWidth, textHeight};
      SDL_RenderCopy(Game::renderer, textTexture, nullptr, &textRect);
    }
  }

  void handleEvent(SDL_Event* e) {
    if (e->type == SDL_MOUSEBUTTONDOWN) {
      int x, y;
      SDL_GetMouseState(&x, &y);
      if (x >= buttonRect.x && x <= buttonRect.x + buttonRect.w &&
          y >= buttonRect.y && y <= buttonRect.y + buttonRect.h) {
        onClick();
      }
    }
  }

 private:
  SDL_Rect buttonRect;
  SDL_Color buttonColor;
  SDL_Color textColor;
  std::string text;
  TTF_Font* font;
  SDL_Texture* textTexture;
  std::function<void()> onClick;

  void createTextTexture() {
    if (textTexture) {
      SDL_DestroyTexture(textTexture);
    }
    SDL_Surface* textSurface =
        TTF_RenderText_Solid(font, text.c_str(), textColor);
    textTexture = SDL_CreateTextureFromSurface(Game::renderer, textSurface);
    SDL_FreeSurface(textSurface);
  }
};

#endif  // BUTTONCOMPONENT_HPP