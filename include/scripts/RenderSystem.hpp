// C++
// File: `include/scripts/RenderSystem.hpp`
#pragma once
#include "../Game.hpp"
#include "Components.hpp" // game Sprite/Tile/Animated components
#include "../../engine/systems/RenderSystem.hpp"
#include <SDL.h>
#include <functional>

#ifndef CAMERA_OFFSET_X
#define CAMERA_OFFSET_X 450
#endif
#ifndef CAMERA_OFFSET_Y
#define CAMERA_OFFSET_Y 250
#endif

class RenderSystem : public engine::systems::RenderSystem {
public:
  void update(Manager& /*mgr*/, float /*dt*/) override {}

  void render(Manager& manager, float /*dt*/) override {
    using engine::render::DrawKey;
    using engine::render::RenderLayer;

    // Use templated helper instantiations for visibility to support multiple component types
    std::function<bool(SpriteComponent*)> spriteVisibleByCamera = &RenderSystem::compVisibleByCamera<SpriteComponent>;
    std::function<bool(AnimatedSpriteComponent*)> animVisibleByCamera = &RenderSystem::compVisibleByCamera<AnimatedSpriteComponent>;

    // Collect all renderables into a single array (tiles, sprites, animated) so we can sort across types
    std::vector<DrawKey> keys;
    std::vector<DrawCallback> callbacks;
    keys.reserve(512);
    callbacks.reserve(512);

    // Tiles (Background)
    for (auto &tpl : manager.each<TileComponent>()) {
      TileComponent* t = std::get<0>(tpl);
      if (!t) continue;
      keys.push_back(DrawKey{ RenderLayer::Background, 0 });
      callbacks.push_back([t]() { t->draw(); });
    }

    // Sprites (Entities)
    for (auto &tpl : manager.each<SpriteComponent>()) {
      SpriteComponent* s = std::get<0>(tpl);
      if (!s) continue;
      if (spriteVisibleByCamera && !spriteVisibleByCamera(s)) continue;
      DrawKey key{ RenderLayer::Entities, 0 };
      Entity* e = s->getEntity();
      if (e && e->hasComponent<RenderComponent>()) {
        auto &r = e->getComponent<RenderComponent>();
        key = DrawKey{ r.layer, r.zOffset };
      }
      keys.push_back(key);
      callbacks.push_back([s]() { s->draw(); });
    }

    // Animated sprites (Entities)
    for (auto &tpl : manager.each<AnimatedSpriteComponent>()) {
      AnimatedSpriteComponent* a = std::get<0>(tpl);
      if (!a) continue;
      if (animVisibleByCamera && !animVisibleByCamera(a)) continue;
      DrawKey key{ RenderLayer::Entities, 0 };
      Entity* e = a->getEntity();
      if (e && e->hasComponent<RenderComponent>()) {
        auto &r = e->getComponent<RenderComponent>();
        key = DrawKey{ r.layer, r.zOffset };
      }
      keys.push_back(key);
      callbacks.push_back([a]() { a->draw(); });
    }

    // Now render everything in a single sorted pass
    if (!keys.empty()) this->renderSorted(keys, callbacks);


  }

private:
  template<typename Comp>
  static bool compVisibleByCamera(Comp* comp) {
    if (!comp) return false;
    Entity* e = comp->getEntity();
    if (!e) return false;
    if (!e->hasComponent<TransformComponent>()) return false;
    const auto &t = e->getComponent<TransformComponent>();
    SDL_Rect dst{
      static_cast<int>(t.position.x - Game::camera.getX() + CAMERA_OFFSET_X),
      static_cast<int>(t.position.y - Game::camera.getY() + CAMERA_OFFSET_Y),
      t.width * t.scale,
      t.height * t.scale
    };
    // return Game::camera.rectIntersects(dst);
    return true;
  }
};
