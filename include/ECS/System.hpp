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
};

