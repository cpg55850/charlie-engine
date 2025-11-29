#pragma once

#include "../../engine/core/System.hpp"
#include "../../engine/core/Manager.hpp"
#include "../../engine/ui/HUD.hpp"
#include "../components/PlayerComponent.hpp"
#include "../../include/Game.hpp"
#include <iostream>

// Game-specific HUD system: pulls player state and updates/draws the engine-agnostic HUD
class GameHUDSystem : public System {
public:
    void update(Manager& /*manager*/, float /*dt*/) override {
        // No per-frame logic here; HUD is drawn in render phase
    }

    void render(Manager& manager, float /*dt*/) override {
        // std::cerr << "GameHUDSystem::render called\n";
        auto& players = manager.getGroup(Game::groupPlayers);
        if (!players.empty()) {
            Entity* player = players.front();
            if (player && player->hasComponent<PlayerComponent>()) {
                int health = player->getComponent<PlayerComponent>().health;
                std::string txt = "Health: " + std::to_string(health);
                // std::cerr << "GameHUDSystem: setText('" << txt << "')\n";
                HUD::setText(txt);
            }
        }
        // Draw the HUD overlay (engine rendering handled here)
        HUD::draw();
    }
};
