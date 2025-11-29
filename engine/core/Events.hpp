#pragma once

class Entity;

// Lightweight collision event data passed to components
struct CollisionEvent {
    Entity* self = nullptr;   // entity receiving the event
    Entity* other = nullptr;  // the other entity involved in the collision
};

