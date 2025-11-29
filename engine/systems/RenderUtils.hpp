// cpp
#pragma once
#include "RenderLayer.hpp"
#include <vector>
#include <algorithm>

namespace engine::render {

    struct DrawKey {
        RenderLayer layer;
        int zOffset;

        // Allows us to compare keys w < operator
        bool operator<(DrawKey const &o) const noexcept {
            if (static_cast<int>(layer) != static_cast<int>(o.layer))
                return static_cast<int>(layer) < static_cast<int>(o.layer);
            return zOffset < o.zOffset;
        }
    };

    // Stable-sort helper for index mapping using keys
    template<typename IndexT>
    inline void stableSortByKeys(std::vector<IndexT> &indices,
                                 const std::vector<DrawKey> &keys) {
        std::stable_sort(indices.begin(), indices.end(),
          [&keys](IndexT a, IndexT b) {
            return keys[a] < keys[b];
          });
    }

} // namespace engine::render
