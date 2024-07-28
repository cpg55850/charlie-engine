#include "Scene.hpp"

// Constructor definition
Scene::Scene(const std::string& name) : name(name) {}

// Getter definition
std::string Scene::getName() const { return name; }
