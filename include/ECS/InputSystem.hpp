#pragma once

#include "System.hpp"
#include "Manager.hpp"
#include "InputComponent.hpp"
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

            if (entity->hasComponent<InputComponent>()) {
                auto& input = entity->getComponent<InputComponent>();

                // Read input directly from SDL and populate InputComponent
                // This is pure ECS - no external managers needed!

                // Movement keys
                input.setInput("MoveRight",
                    keyboardState[SDL_SCANCODE_D] || keyboardState[SDL_SCANCODE_RIGHT]);
                input.setInput("MoveLeft",
                    keyboardState[SDL_SCANCODE_A] || keyboardState[SDL_SCANCODE_LEFT]);
                input.setInput("MoveUp",
                    keyboardState[SDL_SCANCODE_W] || keyboardState[SDL_SCANCODE_UP]);
                input.setInput("MoveDown",
                    keyboardState[SDL_SCANCODE_S] || keyboardState[SDL_SCANCODE_DOWN]);

                // Action keys
                input.setInput("Shoot", keyboardState[SDL_SCANCODE_SPACE]);

                // Add more inputs as needed
                // input.setInput("Jump", keyboardState[SDL_SCANCODE_SPACE]);
                // input.setInput("Interact", keyboardState[SDL_SCANCODE_E]);
            }
        }
    }
};

