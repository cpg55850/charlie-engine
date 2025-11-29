#pragma once

#include "../core/System.hpp"
#include "../core/Manager.hpp"
#include "../components/TransformComponent.hpp"
#include "../components/ColliderComponent.hpp"
#include "../components/SpriteComponent.hpp"
#include "../components/AnimatedSpriteComponent.hpp"

// System that handles movement by updating entity positions based on velocity
class MovementSystem : public System {
public:
    void update(Manager& manager, float deltaTime) override {
        // Update transforms and sync colliders when present
        auto list = manager.each<TransformComponent>();
        for (auto& tpl : list) {
            TransformComponent* transform = std::get<0>(tpl);
            Entity* owner = transform->entity;
            if (!owner || !owner->isActive()) continue;

            transform->position.x += transform->velocity.x * deltaTime;
            transform->position.y += transform->velocity.y * deltaTime;

            if (owner->hasComponent<ColliderComponent>()) {
                auto& col = owner->getComponent<ColliderComponent>();
                col.collider.x = static_cast<int>(transform->position.x);
                col.collider.y = static_cast<int>(transform->position.y);
                col.collider.w = transform->width * transform->scale;
                col.collider.h = transform->height * transform->scale;
            }
        }

        // Update static sprites
        auto sprites = manager.each<SpriteComponent>();
        for (auto& tpl : sprites) {
            SpriteComponent* s = std::get<0>(tpl);
            s->update(deltaTime);
        }

        // Update animated sprites
        auto anims = manager.each<AnimatedSpriteComponent>();
        for (auto& tpl : anims) {
            AnimatedSpriteComponent* a = std::get<0>(tpl);
            a->update(deltaTime);
        }
    }
};
