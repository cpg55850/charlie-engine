#pragma once
#include "../../engine/core/System.hpp"
#include "../../engine/core/Manager.hpp"
#include "../../engine/components/TransformComponent.hpp"
#include "../components/EnemyComponent.hpp"
#include "components/PlayerComponent.hpp"
#include <cmath>
#include <limits>

class EnemyAISystem : public System {
public:
    void update(Manager& manager, float dt) override {
        // Get player transforms once using the new view helper
        auto playerTuples = manager.view<TransformComponent, PlayerComponent>();
        if (playerTuples.empty()) return;

        // Build a compact list of player transforms for fast iteration
        std::vector<TransformComponent*> players;
        players.reserve(playerTuples.size());
        for (auto& tpl : playerTuples) players.push_back(std::get<0>(tpl));

        // Iterate enemy entities via view to get (Transform, Enemy) pairs directly
        auto enemyTuples = manager.view<TransformComponent, EnemyComponent>();
        for (auto& tpl : enemyTuples) {
            TransformComponent* et = std::get<0>(tpl);
            EnemyComponent* ec = std::get<1>(tpl);
            if (!ec->chasePlayer) continue;

            // Find nearest player (squared distance)
            TransformComponent* nearest = nullptr;
            float nearestDist2 = std::numeric_limits<float>::max();
            for (auto* pt : players) {
                float dx = pt->position.x - et->position.x;
                float dy = pt->position.y - et->position.y;
                float d2 = dx*dx + dy*dy;
                if (d2 < nearestDist2) { nearestDist2 = d2; nearest = pt; }
            }
            if (!nearest) { et->velocity.x = et->velocity.y = 0.0f; continue; }

            float dx = nearest->position.x - et->position.x;
            float dy = nearest->position.y - et->position.y;
            float len = std::sqrt(dx*dx + dy*dy);
            if (len > 1.0f) {
                et->velocity.x = (dx / len) * ec->speed;
                et->velocity.y = (dy / len) * ec->speed;
            } else {
                et->velocity.x = et->velocity.y = 0.0f; // reached
            }
        }
    }
};
