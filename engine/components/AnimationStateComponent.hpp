#pragma once

#include "../core/Component.hpp"

// Component that stores animation state
// AnimationSystem will read this to choose which animation to play
class AnimationStateComponent : public Component {
public:
    enum Direction { NONE, UP, DOWN, LEFT, RIGHT };

    Direction facingDirection = RIGHT;
    bool isMoving = false;
    bool isAttacking = false;
    bool isJumping = false;

    // Animation speed multiplier (for run vs walk)
    float speedMultiplier = 1.0f;

    AnimationStateComponent() = default;
};

