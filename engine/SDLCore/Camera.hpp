#pragma once

#include <SDL.h>

class Camera {
 public:
  Camera(int x, int y, int width, int height, int worldWidth, int worldHeight);

  void update(int playerX, int playerY, int playerWidth, int playerHeight);

  void render(int playerX, int playerY);

  int getX() const { return x; }
  int getY() const { return y; }
  int getWidth() const { return width; }
  int getHeight() const { return height; }

  // Check if an SDL_Rect intersects the camera view
  bool rectIntersects(const SDL_Rect& r) const;

 private:
  float x, y;
  int width, height;
  int worldWidth, worldHeight;
};
