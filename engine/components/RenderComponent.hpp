#pragma once

#include "../core/Component.hpp"
#include "../systems/RenderLayer.hpp"

class RenderComponent : public Component {
public:
    RenderComponent() = default;
    RenderComponent(engine::render::RenderLayer layer_, int zOffset_, bool visible_ = true)
        : layer(layer_), zOffset(zOffset_), visible(visible_) {}

    engine::render::RenderLayer layer = engine::render::RenderLayer::Entities;
    int zOffset = 0;
    bool visible = true;
};

