#pragma once

#include "System.hpp"
#include "Manager.hpp"
#include "InputComponent.hpp"
#include "PlayerIdComponent.hpp"
#include <SDL.h>

// System that handles input processing directly from SDL
// No InputManager needed - reads keyboard state directly
class InputSystem : public System {
public:
    void update(Manager& manager, float deltaTime) override {
        // Get current keyboard state from SDL
        const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

        // Populate InputComponent for all entities that have one
        for (auto& entity : manager.getEntities()) {
            if (!entity->isActive()) continue;
            if (!entity->hasComponent<InputComponent>()) continue;

            auto& input = entity->getComponent<InputComponent>();
            int pid = 0;
            if (entity->hasComponent<PlayerIdComponent>()) {
                pid = entity->getComponent<PlayerIdComponent>().id;
            }
            // Clear previous frame states (basic approach)
            input.pressedInputs.clear();

            if (pid == 0) {
                input.setInput("MoveRight", keyboardState[SDL_SCANCODE_D]);
                input.setInput("MoveLeft",  keyboardState[SDL_SCANCODE_A]);
                input.setInput("MoveUp",    keyboardState[SDL_SCANCODE_W]);
                input.setInput("MoveDown",  keyboardState[SDL_SCANCODE_S]);
                input.setInput("Shoot",     keyboardState[SDL_SCANCODE_SPACE]);
            } else if (pid == 1) {
                input.setInput("MoveRight", keyboardState[SDL_SCANCODE_RIGHT]);
                input.setInput("MoveLeft",  keyboardState[SDL_SCANCODE_LEFT]);
                input.setInput("MoveUp",    keyboardState[SDL_SCANCODE_UP]);
                input.setInput("MoveDown",  keyboardState[SDL_SCANCODE_DOWN]);
                input.setInput("Shoot",     keyboardState[SDL_SCANCODE_RCTRL] || keyboardState[SDL_SCANCODE_RSHIFT]);
            }
        }
    }
};
