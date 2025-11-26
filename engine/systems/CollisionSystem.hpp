#pragma once

#include "../core/System.hpp"
#include "../core/Manager.hpp"
#include "../components/TransformComponent.hpp"
#include "../components/ColliderComponent.hpp"
#include "../SDLCore/Collision.hpp"

// Project-agnostic CollisionSystem: keeps collider rectangles in sync with TransformComponent
// and performs basic overlap detection without any game-specific response.
class CollisionSystem : public System {
public:
    void update(Manager& manager, float /*deltaTime*/) override {
        auto& entities = manager.getEntities();

        // Update collider positions from transforms (generic behavior)
        for (auto& entity : entities) {
            if (!entity->isActive()) continue;
            if (entity->hasComponent<TransformComponent>() && entity->hasComponent<ColliderComponent>()) {
                auto& transform = entity->getComponent<TransformComponent>();
                auto& collider = entity->getComponent<ColliderComponent>();
                collider.collider.x = static_cast<int>(transform.position.x);
                collider.collider.y = static_cast<int>(transform.position.y);
                collider.collider.w = transform.width * transform.scale;
                collider.collider.h = transform.height * transform.scale;
            }
        }

        // Detect overlapping collider pairs and invoke a hook for project-specific handling.
        // Note: this performs O(n^2) checks; projects can replace/extend with spatial partitioning.
        const size_t n = entities.size();
        for (size_t i = 0; i < n; ++i) {
            Entity* a = entities[i].get();
            if (!a->isActive() || !a->hasComponent<ColliderComponent>()) continue;
            for (size_t j = i + 1; j < n; ++j) {
                Entity* b = entities[j].get();
                if (!b->isActive() || !b->hasComponent<ColliderComponent>()) continue;
                auto& ca = a->getComponent<ColliderComponent>();
                auto& cb = b->getComponent<ColliderComponent>();
                if (Collision::AABB(ca, cb)) {
                    onOverlap(a, b);
                }
            }
        }
    }

protected:
    // Projects should override this to implement game-specific collision responses.
    virtual void onOverlap(class Entity* /*a*/, class Entity* /*b*/) {}
};
