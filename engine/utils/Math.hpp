#pragma once
#include "Vector2D.hpp"

class Math {
 public:
  static Vector2D calculateDirection(const Vector2D& start,
                                     const Vector2D& end) {
    return end - start;
  }

  static void normalize(Vector2& vec) { vec.normalize(); }
};