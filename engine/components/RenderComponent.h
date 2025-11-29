#include "SDL_render.h"
#include "systems/RenderLayer.hpp"

class RenderComponent {
    SDL_Texture* texture;             // or whatever your sprite type is
    int srcX, srcY, srcW, srcH;  // source rect
    engine::render::RenderLayer layer{engine::render::RenderLayer::Entities}; // default layer
    int zOffset{0};              // optional fine-grain ordering inside a layer

    RenderComponent(const SDL_Texture &tex,
                    int sx = 0, int sy = 0, int sw = 0, int sh = 0,
                    engine::render::RenderLayer l = engine::render::RenderLayer::Entities,
                    int z = 0)
      : texture(tex), srcX(sx), srcY(sy), srcW(sw), srcH(sh), layer(l), zOffset(z) {}
};
