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
        for (auto& entity : manager.getEntities()) {
            if (!entity->isActive()) continue;

            // Background tiles
            if (entity->hasComponent<TileComponent>()) {
                entity->getComponent<TileComponent>().draw();
            }
            // Static sprite
            if (entity->hasComponent<SpriteComponent>()) {
                if (isFlashing(entity.get())) drawFlash(*entity); else entity->getComponent<SpriteComponent>().draw();
            }
            // Animated sprite
            if (entity->hasComponent<AnimatedSpriteComponent>()) {
                if (isFlashing(entity.get())) drawFlash(*entity); else entity->getComponent<AnimatedSpriteComponent>().draw();
            }
#ifdef DEBUG_COLLIDERS
            if (entity->hasComponent<ColliderComponent>()) {
                entity->getComponent<ColliderComponent>().draw();
            }
#endif
        }
    }
private:
    bool isFlashing(Entity* e) const {
        return e->hasComponent<FlashOnHitComponent>() && e->getComponent<FlashOnHitComponent>().flashing && e->hasComponent<TransformComponent>();
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
