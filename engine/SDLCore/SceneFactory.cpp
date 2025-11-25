#include "SceneFactory.hpp"

SceneFactory& SceneFactory::instance() {
  static SceneFactory factory;
  return factory;
}

void SceneFactory::registerScene(const std::string& name,
                                 SceneCreator creator) {
  creators[name] = creator;
}

std::unique_ptr<Scene> SceneFactory::createScene(const std::string& name) {
  auto it = creators.find(name);
  if (it != creators.end()) {
    return it->second();
  }
  return nullptr;
}
