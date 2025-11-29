#pragma once

class Manager;  // Forward declare Manager class

// Base System class that all systems inherit from
class System {
public:
    virtual ~System() = default;

    // Pure virtual update method - all systems must implement this
    virtual void update(Manager& manager, float deltaTime) = 0;

    // Optional initialize method for systems that need setup
    virtual void init() {}

    // Optional per-frame render hook for systems that draw UI or overlays
    virtual void render(Manager& /*manager*/, float /*deltaTime*/) {}

    // Optional shutdown/cleanup hook called when system or engine is shutting down
    virtual void quit() {}
};
