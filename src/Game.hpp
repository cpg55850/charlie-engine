#pragma once
/*This source code copyrighted by Lazy Foo' Productions 2004-2024
and may not be redistributed without written permission.*/

// Using SDL, SDL_image, SDL_ttf, standard IO, strings, and string streams
#include <SDL.h>
// #include <SDL_image.h>
// #include <SDL_ttf.h>
#include <SDL_image.h>
#include <stdio.h>

#include <iostream>

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

 private:
  int count = 0;
  bool isRunning;
  SDL_Window *window;
  SDL_Renderer *renderer;
};