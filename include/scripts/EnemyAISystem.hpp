#pragma once
#include "../../engine/core/System.hpp"
#include "../../engine/core/Manager.hpp"
#include "../../engine/components/TransformComponent.hpp"
#include "../components/EnemyComponent.hpp"
#include "components/PlayerComponent.hpp"
#include <cmath>

class EnemyAISystem : public System {
public:
    void update(Manager& manager, float dt) override {
        // Gather player transforms first
        std::vector<TransformComponent*> players;
        for (auto& e : manager.getEntities()) {
            if (!e->isActive()) continue;
            if (e->hasComponent<PlayerComponent>() && e->hasComponent<TransformComponent>()) {
                players.push_back(&e->getComponent<TransformComponent>());
            }
        }
        if (players.empty()) return;

        // For each enemy, chase nearest player
        for (auto& e : manager.getEntities()) {
            if (!e->isActive()) continue;
            if (!e->hasComponent<EnemyComponent>() || !e->hasComponent<TransformComponent>()) continue;
            auto& ec = e->getComponent<EnemyComponent>();
            if (!ec.chasePlayer) continue;
            auto& et = e->getComponent<TransformComponent>();

            // Find nearest player
            TransformComponent* nearest = nullptr;
            float nearestDist2 = std::numeric_limits<float>::max();
            for (auto* pt : players) {
                float dx = pt->position.x - et.position.x;
                float dy = pt->position.y - et.position.y;
                float d2 = dx*dx + dy*dy;
                if (d2 < nearestDist2) { nearestDist2 = d2; nearest = pt; }
            }
            if (!nearest) continue;
            float dx = nearest->position.x - et.position.x;
            float dy = nearest->position.y - et.position.y;
            float len = std::sqrt(dx*dx + dy*dy);
            if (len > 1.0f) {
                et.velocity.x = (dx / len) * ec.speed;
                et.velocity.y = (dy / len) * ec.speed;
            } else {
                et.velocity.x = et.velocity.y = 0; // reached
            }
        }
    }
};
