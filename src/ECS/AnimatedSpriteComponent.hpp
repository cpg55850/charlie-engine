#pragma once

#include <map>

#include "../TextureManager.hpp"
#include "Animation.hpp"
#include "Components.hpp"
#include "SDL.h"
class AnimatedSpriteComponent : public Component {
 private:
  TransformComponent *transform;
  SDL_Texture *texture;
  SDL_Rect srcRect, destRect;

  bool isAnimated = false;
  int frames = 0;
  int speed = 100;

 public:
  int animIndex = 0;

  std::map<const char *, Animation> animations;

  SDL_RendererFlip spriteFlip = SDL_FLIP_NONE;

  AnimatedSpriteComponent() = default;
  AnimatedSpriteComponent(const char *path) { setTex(path); }
  ~AnimatedSpriteComponent() { SDL_DestroyTexture(texture); }

  AnimatedSpriteComponent(const char *path, Animation anim) {
    // Animation walk = Animation(0, 2, 100);

    // animations.emplace("Idle", idle);
    animations.emplace(path, anim);

    play(path);
    setTex(path);
  }

  void setTex(const char *path) { texture = TextureManager::LoadTexture(path); }

  void addTex(const char *path, Animation anim) {
    animations.emplace(path, anim);
  }

  void playTex(const char *path) {
    play(path);
    setTex(path);
  }

  void init() override {
    transform = &entity->getComponent<TransformComponent>();
    srcRect.x = srcRect.y = 0;
    srcRect.w = transform->width;
    srcRect.h = transform->height;
  }
  void update(float deltaTime) override {
    srcRect.x = srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);

    srcRect.y = animIndex * transform->height;

    destRect.x = static_cast<int>(transform->position.x);
    destRect.y = static_cast<int>(transform->position.y);
    destRect.w = transform->width * transform->scale;
    destRect.h = transform->height * transform->scale;
  }
  void draw() override {
    TextureManager::Draw(texture, srcRect, destRect, spriteFlip);
  }

  void play(const char *animName) {
    frames = animations[animName].mFrames;
    animIndex = animations[animName].mIndex;
    speed = animations[animName].mSpeed;
  }
};