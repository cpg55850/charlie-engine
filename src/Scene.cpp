#include "Scene.hpp"
#include "Game.hpp"

// Constructor definition
Scene::Scene(const std::string& name) : name(name) {}

// Destructor definition
Scene::~Scene() {
  for (auto* e : ownedEntities) {
    if (e) e->destroy();
  }
}

// Getter definition
std::string Scene::getName() const { return name; }

Entity* Scene::createEntity() {
  auto& ent = Game::manager.addEntity();
  ownedEntities.push_back(&ent);
  return &ent;
}
