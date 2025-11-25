#pragma once
#include "Entity.hpp"

// Generic helper: add component if missing, return reference either way.
// Usage: auto& t = ensureComponent<TransformComponent>(entity, x, y, w, h, scale);
//        auto& sprite = ensureComponent<AnimatedSpriteComponent>(entity);
// This removes repetitive hasComponent<T>() checks throughout initialization code.

template <typename T, typename... Args>
T& ensureComponent(Entity& e, Args&&... args) {
    if (e.hasComponent<T>()) {
        return e.getComponent<T>();
    }
    return e.addComponent<T>(std::forward<Args>(args)...);
}

