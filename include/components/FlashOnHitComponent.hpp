#pragma once
#include "../../engine/core/Component.hpp"
#include <SDL.h>

struct FlashOnHitComponent : public Component {
    bool flashing = false;
    Uint32 flashStart = 0;
    Uint32 flashDurationMs = 120; // length of flash
    void trigger() { flashing = true; flashStart = SDL_GetTicks(); }
    void update(float) override {
        if (flashing && SDL_GetTicks() - flashStart > flashDurationMs) {
            flashing = false;
        }
    }
};
