#pragma once
#include <stdio.h>

#include <string>
#include <vector>

class Map {
 public:
  Map();
  ~Map();

  static std::vector<std::string> splitString(const std::string &s,
                                              char delimiter);
  static void LoadMap(std::string path, int sizeX, int sizeY);

 private:
};