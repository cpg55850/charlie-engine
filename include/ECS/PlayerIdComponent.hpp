#pragma once
#include "Component.hpp"

struct PlayerIdComponent : public Component {
    int id = 0;
    PlayerIdComponent() = default;
    explicit PlayerIdComponent(int i) : id(i) {}
};
