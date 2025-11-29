#pragma once
#include "../core/System.hpp"
#include "../core/Manager.hpp"
#include "../components/CameraFollowComponent.hpp"
#include "../components/TransformComponent.hpp"
#include "Game.hpp"

class CameraFollowSystem : public System {
public:
    void update(Manager& manager, float /*deltaTime*/) override {
        auto list = manager.each<CameraFollowComponent, TransformComponent>();
        for (auto& tpl : list) {
            CameraFollowComponent* follow = std::get<0>(tpl);
            TransformComponent* tr = std::get<1>(tpl);
            Entity* owner = follow->entity;
            if (!owner || !owner->isActive()) continue;
            if (!follow->enabled) continue;
            int camX = static_cast<int>(tr->position.x + follow->offsetX);
            int camY = static_cast<int>(tr->position.y + follow->offsetY);
            Game::camera.update(camX, camY, tr->width, tr->height);
            break; // follow only first
        }
    }
};
