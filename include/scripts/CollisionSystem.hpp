#pragma once
// Game-specific CollisionSystem (uses Game::colliders, enemy combat logic)
#include "../ECS/Components.hpp"
#include "../Game.hpp"
#include "../../engine/core/System.hpp"
#include "../../engine/core/Manager.hpp"
#include "../Collision.hpp"
#include "scripts/EnemyComponent.hpp"
#include <iostream>

class CollisionSystem : public System {
public:
    void update(Manager& manager, float dt) override {
        auto& entities = manager.getEntities();
        // Predictive wall collision
        for (auto& e : entities) {
            if (!e->isActive()) continue;
            if (!e->hasComponent<TransformComponent>() || !e->hasComponent<ColliderComponent>()) continue;
            auto& tr = e->getComponent<TransformComponent>();
            float newX = tr.position.x + tr.velocity.x * dt;
            float newY = tr.position.y + tr.velocity.y * dt;
            SDL_Rect predicted{static_cast<int>(newX), static_cast<int>(newY), tr.width * tr.scale, tr.height * tr.scale};
            if (hitsWall(predicted)) {
                tr.velocity.x = 0; tr.velocity.y = 0;
            }
        }
        // Entity-entity collision
        for (size_t i=0;i<entities.size();++i) {
            Entity* a = entities[i].get();
            if (!validCollider(a)) continue;
            for (size_t j=i+1;j<entities.size();++j) {
                Entity* b = entities[j].get();
                if (!validCollider(b)) continue;
                if (Collision::AABB(a->getComponent<ColliderComponent>(), b->getComponent<ColliderComponent>())) {
                    handle(a,b);
                }
            }
        }
    }
private:
    bool validCollider(Entity* e) { return e->isActive() && e->hasComponent<ColliderComponent>(); }
    bool hitsWall(const SDL_Rect& predicted) {
        for (auto* cc : Game::colliders) {
            if (cc->tag == "wall" && Collision::AABB(predicted, cc->collider)) return true;
        }
        return false;
    }
    void handle(Entity* a, Entity* b) {
        auto& ca = a->getComponent<ColliderComponent>();
        auto& cb = b->getComponent<ColliderComponent>();
        bool bulletEnemy = (ca.tag=="projectile" && cb.tag=="enemy") || (cb.tag=="projectile" && ca.tag=="enemy");
        if (!bulletEnemy) return;
        Entity* bullet = (ca.tag=="projectile") ? a : b;
        Entity* enemy  = (ca.tag=="enemy") ? a : b;
        if (enemy->hasComponent<FlashOnHitComponent>()) enemy->getComponent<FlashOnHitComponent>().trigger();
        int dmg = bullet->hasComponent<DamageComponent>() ? bullet->getComponent<DamageComponent>().damage : 1;
        if (enemy->hasComponent<EnemyComponent>()) {
            auto& ec = enemy->getComponent<EnemyComponent>();
            ec.health -= dmg;
            std::cout << "Enemy health now: " << ec.health << std::endl;
            if (ec.health <= 0) enemy->destroy();
        }
        bullet->destroy();
    }
};
