#pragma once

class Entity;  // Forward declare Entity

class Component {
public:
    Entity* entity = nullptr;
    virtual void init() {}        // Initialize component
    virtual void update(float deltaTime) {}  // Update component behavior
    virtual void draw() {}       // Optional: Render behavior
    virtual ~Component() = default;
};