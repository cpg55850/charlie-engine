#pragma once

#include <map>

#include "../TextureManager.hpp"
#include "Animation.hpp"
#include "Components.hpp"
#include "SDL.h"

class SpriteComponent : public Component {
 public:
  SpriteComponent() : transform(nullptr), texture(nullptr) {}
  //  SDL_DestroyTexture(texture);
  ~SpriteComponent() {}

  void init() override {
    transform = &entity->getComponent<TransformComponent>();
    srcRect.x = srcRect.y = 0;
    srcRect.w = transform->width;
    srcRect.h = transform->height;
  }

  void setTex(const char* path) { texture = TextureManager::LoadTexture(path); }

  void draw() override {
    TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
  }

  void update(float deltaTime) override {
    transform = &entity->getComponent<TransformComponent>();

    destRect.x = static_cast<int>(transform->position.x);
    destRect.y = static_cast<int>(transform->position.y);
    destRect.w = transform->width * transform->scale;
    destRect.h = transform->height * transform->scale;
  }

 private:
  TransformComponent* transform;
  SDL_Texture* texture;
  SDL_Rect srcRect, destRect;
  SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;
};