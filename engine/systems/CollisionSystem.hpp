#pragma once

#include "../core/System.hpp"
#include "../core/Manager.hpp"
#include "../components/TransformComponent.hpp"
#include "../components/ColliderComponent.hpp"
#include "../SDLCore/Collision.hpp"
#include <algorithm>
#include <vector>

// Project-agnostic CollisionSystem: keeps collider rectangles in sync with TransformComponent
// and performs overlap detection using a sweep-and-prune broad-phase followed by AABB.
class CollisionSystem : public System {
public:
    void update(Manager& manager, float deltaTime) override {
        auto& entities = manager.getEntities();

        struct Item {
            Entity* ent;
            ColliderComponent* col;
            int left;
            int right;
            int top;
            int bottom;
        };

        std::vector<Item> colliders;
        colliders.reserve(entities.size());

        // 1) Update collider rects from transforms and collect active collider pointers
        for (auto& u : entities) {
            Entity* e = u.get();
            if (!e || !e->isActive()) continue;
            if (!e->hasComponent<TransformComponent>() || !e->hasComponent<ColliderComponent>()) continue;

            auto& t = e->getComponent<TransformComponent>();
            auto& c = e->getComponent<ColliderComponent>();

            // Sync collider to transform (current)
            const int curX = static_cast<int>(t.position.x);
            const int curY = static_cast<int>(t.position.y);
            const int w = t.width * t.scale;
            const int h = t.height * t.scale;
            c.collider.x = curX;
            c.collider.y = curY;
            c.collider.w = w;
            c.collider.h = h;

            // Compute previous position from velocity and deltaTime to form a swept AABB
            const int prevX = static_cast<int>(t.position.x - t.velocity.x * deltaTime);
            const int prevY = static_cast<int>(t.position.y - t.velocity.y * deltaTime);

            Item it{};
            it.ent = e;
            it.col = &c;
            it.left = std::min(curX, prevX);
            it.right = std::max(curX + w, prevX + w);
            it.top = std::min(curY, prevY);
            it.bottom = std::max(curY + h, prevY + h);
            colliders.push_back(it);
        }

        if (colliders.empty()) return;

        // 2) Sort by left edge (x)
        std::sort(colliders.begin(), colliders.end(), [](const Item& a, const Item& b) {
            return a.left < b.left;
        });

        // 3) Sweep: for each collider, only test subsequent colliders whose left <= current right
        const size_t N = colliders.size();
        for (size_t i = 0; i < N; ++i) {
            const Item& A = colliders[i];
            for (size_t j = i + 1; j < N; ++j) {
                const Item& B = colliders[j];

                // Early-out: B is strictly to the right of A -> no more intersections for this A
                if (B.left > A.right) break;

                // Quick vertical overlap rejection before AABB (cheap integer checks)
                if (B.top > A.bottom || B.bottom < A.top) continue;

                // Final precise AABB test (uses component rects)
                if (Collision::AABB(*A.col, *B.col)) {
                    onOverlap(A.ent, B.ent);
                }
            }
        }
    }

protected:
    // Projects should override this to implement game-specific collision responses.
    virtual void onOverlap(class Entity* /*a*/, class Entity* /*b*/) {}
};
