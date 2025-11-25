#pragma once
#include "../core/System.hpp"
#include "../core/Manager.hpp"
#include "../components/CameraFollowComponent.hpp"
#include "../components/TransformComponent.hpp"
#include "Game.hpp"

class CameraFollowSystem : public System {
public:
    void update(Manager& manager, float /*deltaTime*/) override {
        // Find first entity with CameraFollowComponent + TransformComponent
        for (auto& ePtr : manager.getEntities()) {
            if (!ePtr->isActive()) continue;
            if (ePtr->hasComponent<CameraFollowComponent>() && ePtr->hasComponent<TransformComponent>()) {
                auto& follow = ePtr->getComponent<CameraFollowComponent>();
                if (!follow.enabled) continue;
                auto& tr = ePtr->getComponent<TransformComponent>();
                int camX = static_cast<int>(tr.position.x + follow.offsetX);
                int camY = static_cast<int>(tr.position.y + follow.offsetY);
                Game::camera.update(camX, camY, tr.width, tr.height);
                break; // follow only first
            }
        }
    }
};

