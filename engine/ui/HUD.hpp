#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class HUD {
public:
    // Initialize HUD with a renderer and font
    static void init(SDL_Renderer* renderer, const std::string& fontPath, int fontSize);
    static void quit();

    // Update the text to render (engine-agnostic)
    static void setText(const std::string& text);

    // Draw current text at the configured position (default top-left)
    static void draw();

    // Optional: specify draw position
    static void setPosition(int x, int y);

private:
    static TTF_Font* font;
    static SDL_Texture* texture;
    static SDL_Renderer* s_renderer;
    static SDL_Rect rect;
    static SDL_Color color;
    static std::string lastText;

    static void updateTexture(const std::string& text);
};
