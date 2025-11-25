#pragma once
#include <iostream>
#include <string>
#include "ECS/Manager.hpp"
#include <vector>

class Scene {
 public:
  Scene(const std::string& name);
  virtual ~Scene();

  virtual void update(float deltaTime) {}
  virtual void draw() {}
  virtual void onEnter() {}
  virtual void onExit() {}

  std::string getName() const;

  // Create and track an entity owned by this scene
  Entity* createEntity();
 protected:
  std::vector<Entity*> ownedEntities; // entities created via createEntity()

 private:
  std::string name;
};
