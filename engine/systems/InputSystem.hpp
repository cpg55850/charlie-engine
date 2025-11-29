#pragma once

#include "../core/System.hpp"
#include "../core/Manager.hpp"
#include "../components/InputComponent.hpp"
#include "../../include/components/PlayerIdComponent.hpp"
#include <SDL.h>

// System that handles input processing directly from SDL
// No InputManager needed - reads keyboard state directly
class InputSystem : public System {
public:
    void update(Manager& manager, float /*deltaTime*/) override {
        // Get current keyboard state from SDL
        const Uint8* keyboardState = SDL_GetKeyboardState(NULL);

        // First, process entities with InputComponent + PlayerIdComponent
        auto withId = manager.each<InputComponent, PlayerIdComponent>();
        for (auto& tpl : withId) {
            InputComponent* input = std::get<0>(tpl);
            PlayerIdComponent* pidComp = std::get<1>(tpl);
            int pid = pidComp ? pidComp->id : 0;
            input->pressedInputs.clear();
            fillInputs(*input, pid, keyboardState);
        }

        // Then, process entities with only InputComponent
        auto onlyInput = manager.each<InputComponent>();
        for (auto& tpl : onlyInput) {
            InputComponent* input = std::get<0>(tpl);
            // Determine pid if present via entity pointer
            Entity* owner = input->entity;
            int pid = 0;
            if (owner && owner->hasComponent<PlayerIdComponent>()) pid = owner->getComponent<PlayerIdComponent>().id;
            input->pressedInputs.clear();
            fillInputs(*input, pid, keyboardState);
        }
    }

private:
    void fillInputs(InputComponent& input, int pid, const Uint8* ks) {
        if (pid == 0) {
            input.setInput("MoveRight", ks[SDL_SCANCODE_D]);
            input.setInput("MoveLeft",  ks[SDL_SCANCODE_A]);
            input.setInput("MoveUp",    ks[SDL_SCANCODE_W]);
            input.setInput("MoveDown",  ks[SDL_SCANCODE_S]);
            input.setInput("Shoot",     ks[SDL_SCANCODE_SPACE]);
        } else {
            input.setInput("MoveRight", ks[SDL_SCANCODE_RIGHT]);
            input.setInput("MoveLeft",  ks[SDL_SCANCODE_LEFT]);
            input.setInput("MoveUp",    ks[SDL_SCANCODE_UP]);
            input.setInput("MoveDown",  ks[SDL_SCANCODE_DOWN]);
            input.setInput("Shoot",     ks[SDL_SCANCODE_RCTRL] || ks[SDL_SCANCODE_RSHIFT]);
        }
    }
};
