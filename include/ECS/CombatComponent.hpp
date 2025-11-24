#pragma once

#include "Component.hpp"
#include "AnimationStateComponent.hpp"

// Component that stores combat state and shoot requests
// CombatSystem will read this and spawn projectiles
class CombatComponent : public Component {
public:
    // Shoot request flags
    bool shootRequested = false;
    AnimationStateComponent::Direction shootDirection = AnimationStateComponent::RIGHT;

    // Weapon properties
    float projectileSpeed = 400.0f;
    float fireRate = 100.0f;  // Milliseconds between shots
    Uint32 lastShootTime = 0;

    // Projectile configuration
    const char* projectileTexture = "assets/bullet.png";
    int projectileWidth = 8;
    int projectileHeight = 8;
    int projectileScale = 1;

    CombatComponent() = default;

    // Request a shot in the given direction
    void requestShoot(AnimationStateComponent::Direction direction) {
        shootRequested = true;
        shootDirection = direction;
    }

    // Check if we can shoot (fire rate check)
    bool canShoot() const {
        Uint32 now = SDL_GetTicks();
        Uint32 elapsed = now - lastShootTime;
        return elapsed >= fireRate;
    }

    // Mark shot as fired
    void markShot() {
        shootRequested = false;
        lastShootTime = SDL_GetTicks();
    }
};

