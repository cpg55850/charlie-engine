#pragma once

#include "../ECS/Components.hpp"  // Include any components you need
#include "../Scene.hpp"

class MainMenu : public Scene {
 public:
  MainMenu();
  ~MainMenu();

  void update() override;   // Update logic
  void draw() override;     // Render the scene
  void onEnter() override;  // Load assets, initialize entities
  void onExit() override;   // Load assets, initialize entities

 private:
  // Store your entities and components
  std::vector<std::unique_ptr<Entity>> entities;
  // Add any additional members as needed
};
