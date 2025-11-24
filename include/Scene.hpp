#pragma once
#include <iostream>
#include <string>

class Scene {
 public:
  Scene(const std::string& name);
  virtual ~Scene() = default;

  virtual void update(float deltaTime) {}
  virtual void draw() {}
  virtual void onEnter() {}
  virtual void onExit() {}

  std::string getName() const;

 private:
  std::string name;
};
