#pragma once

#include <map>

#include "../TextureManager.h"
#include "Animation.h"
#include "Components.h"
#include "SDL.h"
class SpriteComponent : public Component {
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

  SpriteComponent() = default;
  SpriteComponent(const char *path) { setTex(path); }
  ~SpriteComponent() { SDL_DestroyTexture(texture); }

  SpriteComponent(const char *path, bool _isAnimated, Animation anim,
                  const char *animName) {
    isAnimated = _isAnimated;
    // Animation idle = Animation(0, 1, 100);
    // Animation walk = Animation(0, 2, 100);

    // animations.emplace("Idle", idle);
    animations.emplace(animName, anim);

    play(animName);
    setTex(path);
  }

  void setTex(const char *path) { texture = TextureManager::LoadTexture(path); }

  void addTex(const char *path, bool _isAnimated, Animation anim,
              const char *animName) {
    isAnimated = _isAnimated;
    // Animation idle = Animation(0, 1, 100);
    // Animation walk = Animation(0, 2, 100);

    // animations.emplace("Idle", idle);
    animations.emplace(animName, anim);

    // play(animName);
    // setTex(path);
  }

  void playTex(const char *path, const char *animName) {
    // isAnimated = _isAnimated;
    // // Animation idle = Animation(0, 1, 100);
    // // Animation walk = Animation(0, 2, 100);

    // // animations.emplace("Idle", idle);
    // animations.emplace(animName, anim);

    play(animName);
    setTex(path);
  }

  void init() override {
    transform = &entity->getComponent<TransformComponent>();
    srcRect.x = srcRect.y = 0;
    srcRect.w = transform->width;
    srcRect.h = transform->height;
  }
  void update() override {
    if (isAnimated) {
      srcRect.x =
          srcRect.w * static_cast<int>((SDL_GetTicks() / speed) % frames);
    }

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