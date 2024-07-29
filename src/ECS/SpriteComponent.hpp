#pragma once

#include <map>

#include "../TextureManager.hpp"
#include "Animation.hpp"
#include "Components.hpp"
#include "SDL.h"

class SpriteComponent : public Component {
 public:
  SpriteComponent(const char* path) { setTex(path); }

  void setTex(const char* path) { texture = TextureManager::LoadTexture(path); }

  void draw() override {
    // Drawing logic for the sprite
  }

 private:
  SDL_Texture* texture;
};