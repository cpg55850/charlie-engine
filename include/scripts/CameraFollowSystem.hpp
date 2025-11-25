#pragma once
// Game-specific CameraFollowSystem using Game::camera
#include "../../engine/core/System.hpp"
#include "../../engine/core/Manager.hpp"
#include "../ECS/Components.hpp"
#include "../Game.hpp"

class CameraFollowSystem : public System {
public:
    void update(Manager& manager, float) override {
        for (auto& e : manager.getEntities()) {
            if (!e->isActive()) continue;
            if (!e->hasComponent<CameraFollowComponent>() || !e->hasComponent<TransformComponent>()) continue;
            auto& follow = e->getComponent<CameraFollowComponent>();
            if (!follow.enabled) continue;
            auto& tr = e->getComponent<TransformComponent>();
            int camX = static_cast<int>(tr.position.x + follow.offsetX);
            int camY = static_cast<int>(tr.position.y + follow.offsetY);
            Game::camera.update(camX, camY, tr.width, tr.height);
            break; // follow first
        }
    }
};
