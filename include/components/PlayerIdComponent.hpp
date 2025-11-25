#pragma once
#include "../../engine/core/Component.hpp"

struct PlayerIdComponent : public Component {
    int id = 0;
    PlayerIdComponent() = default;
    explicit PlayerIdComponent(int i) : id(i) {}
};
