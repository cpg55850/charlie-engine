#pragma once

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Scene.hpp"

class SceneManager {
 public:
  SceneManager() = default;
  ~SceneManager() = default;

  // Load a new scene
  void loadScene(const std::string& sceneName);

  // Switch to a different scene
  void switchScene(const std::string& sceneName);

  // Update the current scene
  void update(float deltaTime);

  // Draw the current scene
  void draw();

  // Get the current scene
  Scene* getCurrentScene() const;

 private:
  // Container for scene objects
  std::unordered_map<std::string, std::unique_ptr<Scene>> scenes;

  // Pointer to the currently active scene
  Scene* currentScene = nullptr;
};
