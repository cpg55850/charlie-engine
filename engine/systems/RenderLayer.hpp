#pragma once
namespace engine::render {
    enum class RenderLayer : int {
        Background = 0,
        World = 100,
        Entities = 200,
        Overlays = 300,
        UI = 400,
        Debug = 500
    };
}
