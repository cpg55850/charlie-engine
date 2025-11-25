#pragma once
// Game-specific InputSystem mapping WASD + arrows
#include "../../engine/core/System.hpp"
#include "../../engine/core/Manager.hpp"
#include "Components.hpp"
#include <SDL.h>

class InputSystem : public System {
public:
    void update(Manager& manager, float) override {
        const Uint8* ks = SDL_GetKeyboardState(nullptr);
        for (auto& e : manager.getEntities()) {
            if (!e->isActive() || !e->hasComponent<InputComponent>()) continue;
            auto& input = e->getComponent<InputComponent>();
            int pid = e->hasComponent<PlayerIdComponent>() ? e->getComponent<PlayerIdComponent>().id : 0;
            input.pressedInputs.clear();
            if (pid==0) {
                input.setInput("MoveRight", ks[SDL_SCANCODE_D]);
                input.setInput("MoveLeft",  ks[SDL_SCANCODE_A]);
                input.setInput("MoveUp",    ks[SDL_SCANCODE_W]);
                input.setInput("MoveDown",  ks[SDL_SCANCODE_S]);
                input.setInput("Shoot",     ks[SDL_SCANCODE_SPACE]);
            } else if (pid==1) {
                input.setInput("MoveRight", ks[SDL_SCANCODE_RIGHT]);
                input.setInput("MoveLeft",  ks[SDL_SCANCODE_LEFT]);
                input.setInput("MoveUp",    ks[SDL_SCANCODE_UP]);
                input.setInput("MoveDown",  ks[SDL_SCANCODE_DOWN]);
                input.setInput("Shoot",     ks[SDL_SCANCODE_RCTRL] || ks[SDL_SCANCODE_RSHIFT]);
            }
        }
    }
};
