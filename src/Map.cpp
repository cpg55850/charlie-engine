#include "Map.h"

#include <fstream>
#include <iostream>
#include <string>

#include "Game.h"

Map::Map() {}

Map::~Map() {}

void Map::LoadMap(std::string path, int sizeX, int sizeY) {
  std::cout << "We reached the load map function" << std::endl;
  char tile;
  std::fstream mapFile;
  mapFile.open(path);

  for (int y = 0; y < sizeY; y++) {
    for (int x = 0; x < sizeX; x++) {
      mapFile.get(tile);  // placed into tile
      Game::AddTile(atoi(&tile), x * 32, y * 32);
      mapFile.ignore();
    }
  }

  mapFile.close();
}
