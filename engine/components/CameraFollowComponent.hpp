#pragma once
#include "../core/Component.hpp"
#include "TransformComponent.hpp"

class CameraFollowComponent : public Component {
public:
    // Pixel offsets from entity center
    int offsetX = 0;
    int offsetY = 0;
    bool enabled = true;

    CameraFollowComponent() = default;
    CameraFollowComponent(int ox, int oy) : offsetX(ox), offsetY(oy) {}
};

