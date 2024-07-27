#pragma once
/*This source code copyrighted by Lazy Foo' Productions 2004-2024
and may not be redistributed without written permission.*/

// Using SDL, SDL_image, SDL_ttf, standard IO, strings, and string streams
#include <SDL.h>
// #include <SDL_image.hpp>
// #include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>

#include <iostream>
#include <vector>

#include "Camera.hpp"
#include "ECS/ECS.hpp"

class ColliderComponent;

// #include <sstream>
// #include <string>

class Game {
 public:
  Game();
  ~Game();

  void init(const char *title, int xpos, int ypos, int width, int height,
            bool fullscreen);

  void handleEvents();
  void update();
  void render();
  void clean();

  bool running() { return isRunning; }

  static Manager manager;
  static void AddTile(int srcX, int srcY, int xPos, int yPos);
  static SDL_Renderer *renderer;
  static SDL_Event event;
  static std::vector<ColliderComponent *> colliders;
  static Camera camera;
  enum groupLabels : std::size_t {
    groupMap,
    groupPlayers,
    groupEnemies,
    groupColliders
  };

 private:
  int count = 0;
  bool isRunning;
  SDL_Window *window;
};