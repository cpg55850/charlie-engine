#pragma once
// System that expires FlashOnHitComponent flashing state after duration
#include "../../engine/core/System.hpp"
#include "../../engine/core/Manager.hpp"
#include "Components.hpp"
#include <SDL.h>

class FlashSystem : public System {
public:
    void update(Manager& manager, float /*dt*/) override {
        Uint32 now = SDL_GetTicks();
        for (auto& e : manager.getEntities()) {
            if (!e->isActive()) continue;
            if (!e->hasComponent<FlashOnHitComponent>()) continue;
            auto& flash = e->getComponent<FlashOnHitComponent>();
            if (flash.flashing && (now - flash.flashStart) > flash.flashDurationMs) {
                flash.flashing = false; // auto-expire
            }
        }
    }
};
