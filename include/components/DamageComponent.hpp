#pragma once
#include "../../engine/core/Component.hpp"

struct DamageComponent : public Component {
    int damage = 1;
    DamageComponent() = default;
    explicit DamageComponent(int d) : damage(d) {}
};
