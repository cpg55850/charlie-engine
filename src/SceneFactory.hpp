#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "Scene.hpp"

class SceneFactory {
 public:
  using SceneCreator = std::function<std::unique_ptr<Scene>()>;

  static SceneFactory& instance();

  void registerScene(const std::string& name, SceneCreator creator);
  std::unique_ptr<Scene> createScene(const std::string& name);

 private:
  SceneFactory() = default;
  std::unordered_map<std::string, SceneCreator> creators;
};
