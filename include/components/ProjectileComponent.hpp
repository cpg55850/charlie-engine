#pragma once

#include "../../engine/core/Component.hpp"

// Simple component marking a projectile and storing its owner for friendly-fire checks
class ProjectileComponent : public Component {
public:
    Entity* owner = nullptr;

    ProjectileComponent() = default;
    explicit ProjectileComponent(Entity* o) : owner(o) {}
};

