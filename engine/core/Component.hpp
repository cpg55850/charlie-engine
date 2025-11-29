#pragma once

class Entity;  // Forward declare Entity

#include "Events.hpp"

class Component {
public:
    Entity* entity = nullptr;
    virtual void init() {}        // Initialize component
    virtual void update(float deltaTime) {}  // Update component behavior
    virtual void draw() {}       // Optional: Render behavior

    // Return the owning Entity (nullable)
    Entity* getEntity() const noexcept { return entity; }

    // Convenience: access other components from the owner.
    // These templates are declared here and defined where Entity is complete.
    template<typename T>
    T& getComponent();

    template<typename T>
    const T& getComponent() const;

    // Helper to test ownership
    bool hasEntity() const noexcept { return entity != nullptr; }

    // Event hook: components can override to receive collision events
    virtual void onCollision(const CollisionEvent& /*event*/) {}

    virtual ~Component() = default;
};

// Include template implementations (requires Entity to be declared before including this header)
#include "Component.inl"
