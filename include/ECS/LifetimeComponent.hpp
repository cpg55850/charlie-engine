#pragma once

#include "Component.hpp"

// Component that tracks entity lifetime
// Useful for bullets, particles, temporary effects
class LifetimeComponent : public Component {
public:
    float maxLifetime = 5.0f;  // Seconds before auto-destroy
    float currentLifetime = 0.0f;

    bool destroyOffScreen = true;  // Destroy if off-screen
    float screenBounds = 2000.0f;  // How far off-screen before destroy

    LifetimeComponent() = default;
    LifetimeComponent(float lifetime) : maxLifetime(lifetime) {}
};

