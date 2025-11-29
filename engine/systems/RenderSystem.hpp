// cpp
#pragma once
#include "../core/System.hpp"
#include "../core/Manager.hpp"
#include "RenderLayer.hpp"
#include "RenderUtils.hpp"
#include <vector>
#include <functional>
#include <stdexcept>

namespace engine::systems {

// Engine-agnostic render system base.
// Provides renderSorted and a templated renderComponents helper for game code.
class RenderSystem : public System {
public:
  using DrawCallback = std::function<void()>;

protected:
  // Sorts draw callbacks by the provided keys and executes them in order.
  void renderSorted(const std::vector<render::DrawKey> &keys,
                    const std::vector<DrawCallback> &callbacks) {
    if (keys.size() != callbacks.size()) {
      throw std::invalid_argument("RenderSystem::renderSorted - keys and callbacks size mismatch");
    }

    std::vector<size_t> indices(keys.size());
    for (size_t i = 0; i < indices.size(); ++i) indices[i] = i;
    engine::render::stableSortByKeys(indices, keys);

    for (size_t idx : indices) {
      if (callbacks[idx]) callbacks[idx]();
    }
  }

  // Templated helper: collects all Comp instances, builds keys and callbacks,
  // then sorts and draws them. Caller may supply:
  //  - keyFor(comp\*) -> DrawKey  (optional)
  //  - drawFunc(comp\*) -> void   (optional, defaults to comp->draw())
  // template<typename Comp>
  // void renderComponents(Manager &manager,
  //                       std::function<render::DrawKey(Comp*)> keyFor = nullptr,
  //                       std::function<bool(Comp*)> isVisible = nullptr,
  //                       std::function<void(Comp*)> drawFunc = nullptr) {
  //   std::vector<engine::render::DrawKey> keys;
  //   std::vector<DrawCallback> callbacks;
  //   keys.reserve(128);
  //   callbacks.reserve(128);
  //
  //   auto view = manager.each<Comp>();
  //   for (auto &tpl : view) {
  //     Comp *c = std::get<0>(tpl);
  //     if (!c) continue;
  //     // Visibility check
  //     if (isVisible && !isVisible(c)) continue;
  //     render::DrawKey key = keyFor
  //       ? keyFor(c)
  //       : render::DrawKey{render::RenderLayer::Entities, 0};
  //     keys.push_back(key);
  //
  //     // capture comp pointer and drawFunc by value
  //     callbacks.push_back([c, drawFunc]() {
  //       if (drawFunc) drawFunc(c);
  //       else c->draw();
  //     });
  //   }
  //
  //   if (!keys.empty()) renderSorted(keys, callbacks);
  // }
};

} // namespace engine::systems
