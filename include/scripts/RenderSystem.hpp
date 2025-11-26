#pragma once
// Game-specific RenderSystem (uses Game::camera, Game::renderer, FlashOnHitComponent)

#include "../Game.hpp"
#include "Components.hpp"
#include "../../engine/core/System.hpp"
#include "../../engine/core/Manager.hpp"
#include <SDL.h>

#ifndef CAMERA_OFFSET_X
#define CAMERA_OFFSET_X 450
#endif
#ifndef CAMERA_OFFSET_Y
#define CAMERA_OFFSET_Y 250
#endif

class RenderSystem : public System {
public:
    void update(Manager& manager, float /*dt*/) override {
        // Draw tiles first
        auto tiles = manager.view<TileComponent>();
        for (auto& tpl : tiles) {
            TileComponent* t = std::get<0>(tpl);
            t->draw();
        }

        // Static sprites
        auto sprites = manager.view<SpriteComponent>();
        for (auto& tpl : sprites) {
            SpriteComponent* s = std::get<0>(tpl);
            Entity* owner = s->entity;
            if (isFlashing(owner)) drawFlash(*owner); else s->draw();
        }

        // Animated sprites
        auto anims = manager.view<AnimatedSpriteComponent>();
        for (auto& tpl : anims) {
            AnimatedSpriteComponent* a = std::get<0>(tpl);
            Entity* owner = a->entity;
            if (isFlashing(owner)) drawFlash(*owner); else a->draw();
        }

#ifdef DEBUG_COLLIDERS
        auto cols = manager.view<ColliderComponent>();
        for (auto& tpl : cols) {
            ColliderComponent* c = std::get<0>(tpl);
            c->draw();
        }
#endif
    }
private:
    bool isFlashing(Entity* e) const {
        return e && e->hasComponent<FlashOnHitComponent>() && e->getComponent<FlashOnHitComponent>().flashing && e->hasComponent<TransformComponent>();
    }
    void drawFlash(Entity& e) {
        auto& t = e.getComponent<TransformComponent>();
        SDL_Rect rect{static_cast<int>(t.position.x - Game::camera.getX() + CAMERA_OFFSET_X),
                      static_cast<int>(t.position.y - Game::camera.getY() + CAMERA_OFFSET_Y),
                      t.width * t.scale, t.height * t.scale};
        Uint8 r,g,b,a; SDL_GetRenderDrawColor(Game::renderer,&r,&g,&b,&a);
        SDL_SetRenderDrawColor(Game::renderer,255,255,255,255);
        SDL_RenderFillRect(Game::renderer,&rect);
        SDL_SetRenderDrawColor(Game::renderer,r,g,b,a);
    }
};
