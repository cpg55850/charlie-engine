#include "HUD.hpp"
#include "../SDLCore/FontLoader.hpp"
#include <iostream>

TTF_Font* HUD::font = nullptr;
SDL_Texture* HUD::texture = nullptr;
SDL_Renderer* HUD::s_renderer = nullptr;
SDL_Rect HUD::rect = {10, 10, 0, 0};
SDL_Color HUD::color = {255, 255, 255, 255};
std::string HUD::lastText = "";

void HUD::init(SDL_Renderer* renderer, const std::string& fontPath, int fontSize) {
    s_renderer = renderer;
    font = FontLoader::loadFont(fontPath, fontSize);
    if (!font) {
        std::cerr << "HUD: failed to load font " << fontPath << "\n";
    } else {
        std::cerr << "HUD: loaded font " << fontPath << " size=" << fontSize << "\n";
    }
}

void HUD::quit() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
    font = nullptr; // FontLoader::clear/quit will handle font
}

void HUD::updateTexture(const std::string& text) {
    if (!font || !s_renderer) return;
    if (text == lastText) return; // no change

    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    SDL_Surface* surf = TTF_RenderText_Solid(font, text.c_str(), color);
    if (!surf) {
        std::cerr << "HUD: failed to create text surface: " << TTF_GetError() << "\n";
        return;
    }
    texture = SDL_CreateTextureFromSurface(s_renderer, surf);
    rect.w = surf->w;
    rect.h = surf->h;
    SDL_FreeSurface(surf);
    lastText = text;
    std::cerr << "HUD: updateTexture -> '" << text << "' (w=" << rect.w << " h=" << rect.h << ")\n";
}

void HUD::setText(const std::string& text) {
    updateTexture(text);
}

void HUD::draw() {
    if (!s_renderer || !texture) {
        // debug
        if (!s_renderer) std::cerr << "HUD: no renderer set\n";
        if (!texture) std::cerr << "HUD: no texture to draw (lastText='" << lastText << "')\n";
        return;
    }
    SDL_RenderCopy(s_renderer, texture, nullptr, &rect);
}

void HUD::setPosition(int x, int y) {
    rect.x = x;
    rect.y = y;
}
