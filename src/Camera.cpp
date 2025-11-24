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