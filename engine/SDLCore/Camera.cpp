#include "Camera.hpp"

#include <iostream>

Camera::Camera(int x, int y, int width, int height, int worldWidth,
               int worldHeight)
    : x(x),
      y(y),
      width(width),
      height(height),
      worldWidth(worldWidth),
      worldHeight(worldHeight) {}

void Camera::update(int playerX, int playerY, int playerWidth,
                    int playerHeight) {
  x = playerX;
  y = playerY;
}

void Camera::render(int playerX, int playerY) {
  x = playerX * 0.01;
  y = playerY * 0.01;
  std::cout << playerX << " " << playerY << std::endl;
}

bool Camera::rectIntersects(const SDL_Rect& r) const {
    int camLeft = static_cast<int>(x) - (width / 2);
    int camTop = static_cast<int>(y) - (height / 2);
    int camRight = camLeft + width;
    int camBottom = camTop + height;

    int rLeft = r.x;
    int rTop = r.y;
    int rRight = r.x + r.w;
    int rBottom = r.y + r.h;

    if (rRight < camLeft) return false;
    if (rLeft > camRight) return false;
    if (rBottom < camTop) return false;
    if (rTop > camBottom) return false;
    return true;
}
