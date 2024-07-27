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
  // Center the camera on the player
  // x = playerX + playerWidth / 2 - width / 2;
  // y = playerY + playerHeight / 2 - height / 2;
  // y = y + 0.01f;
  // x = x + 0.01f;
  x = playerX;
  y = playerY;
  // std::cout << playerX << std::endl;

  // Keep the camera within the bounds of the world
  // if (x < 0) {
  //   x = 0;
  // }
  // if (y < 0) {
  //   y = 0;
  // }
  // if (x + width > worldWidth) {
  //   x = worldWidth - width;
  // }
  // if (y + height > worldHeight) {
  //   y = worldHeight - height;
  // }
}

void Camera::render(int playerX, int playerY) {
  x = playerX * 0.01;
  y = playerY * 0.01;
  std::cout << playerX << " " << playerY << std::endl;
}