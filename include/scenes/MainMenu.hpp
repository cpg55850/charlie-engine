#pragma once

#include "../scripts/Components.hpp"
#include "../../engine/SDLCore/Scene.hpp"
#include <SDL_ttf.h>

class MainMenu : public Scene {
 public:
  MainMenu();
  ~MainMenu();

  void update(float deltaTime) override;  // Calls ECS update + scene-specific logic
  void draw() override;                   // Uses groups (or left empty if RenderSystem does all)
  void onEnter() override;                // Create entities & add components
  void onExit() override;                 // Mark entities for destruction

 private:
  Entity* player = nullptr;
  Entity* player2 = nullptr; // second player instance
  Entity* bullet = nullptr;
  Entity* bullet2 = nullptr;
  Entity* wall = nullptr;
  Entity* enemy = nullptr;
  Entity* button = nullptr;
  Entity* label = nullptr;
  TTF_Font* font = nullptr;
};
