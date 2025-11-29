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
        auto flashes = manager.each<FlashOnHitComponent>();
        for (auto& tpl : flashes) {
            FlashOnHitComponent* flash = std::get<0>(tpl);
            Entity* owner = flash->entity;
            if (!owner || !owner->isActive()) continue;
            if (flash->flashing && (now - flash->flashStart) > flash->flashDurationMs) {
                flash->flashing = false; // auto-expire
            }
        }
    }
};
