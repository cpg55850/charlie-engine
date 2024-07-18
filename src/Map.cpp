#include "Map.h"

#include <stdio.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "Game.h"

Map::Map() {}

Map::~Map() {}

std::vector<std::string> Map::splitString(const std::string &s,
                                          char delimiter) {
  std::vector<std::string> tokens;
  std::stringstream ss(s);
  std::string token;

  while (getline(ss, token, delimiter)) {
    tokens.push_back(token);
  }

  return tokens;
}

void Map::LoadMap(std::string path, int sizeX, int sizeY) {
  std::ifstream infile(
      "assets/WHAT.csv");  // Replace "map.csv" with your file name

  if (!infile) {
    std::cerr << "Error opening file!" << std::endl;
  }

  std::vector<std::vector<int>> matrix;
  std::string line;

  // Read the CSV file into the matrix
  while (std::getline(infile, line)) {
    std::vector<int> row;
    std::stringstream ss(line);
    std::string cell;

    while (std::getline(ss, cell, ',')) {
      row.push_back(std::stoi(cell));  // Convert string to int and add to row
    }

    matrix.push_back(row);  // Add the row to the matrix
  }

  infile.close();

  // Iterate through the matrix and call addTile
  for (size_t y = 0; y < matrix.size(); ++y) {
    for (size_t x = 0; x < matrix[y].size(); ++x) {
      int id = matrix[y][x];
      std::cout << id << std::endl;
      if (id == -1) {
        continue;
      }
      // createTile(id, static_cast<int>(x), static_cast<int>(y));
      Game::AddTile(id, static_cast<int>(x) * 32, static_cast<int>(y) * 32);
    }
  }

  // std::cout << "We reached the load map function" << std::endl;
  // char tile;
  // std::fstream mapFile;
  // mapFile.open(path);

  // if (!mapFile.is_open()) {
  //   std::cerr << "Error: Failed to open file " << path << std::endl;
  //   return;
  // }

  // // Read file contents into a stringstream
  // std::stringstream buffer;
  // buffer << mapFile.rdbuf();
  // std::string mapContents = buffer.str();

  // mapFile.close();

  // std::vector<std::string> myTokens = splitString(mapContents, ',');

  // int x = 0;
  // int y = 0;

  // for (const auto &token : myTokens) {
  // std::cout << token << std::endl;
  // Game::AddTile(x + y, x * 32, y * 32);
  // }

  // for (int y = 0; y < sizeY; y++) {
  // for (int x = 0; x < sizeX; x++) {
  // mapFile.get(tile);  // placed into tile
  // int numTile = tile - '0';
  // std::cout << "the numTile is " << tile << std::endl;
  // Game::AddTile(, x * 32, y * 32);
  // mapFile.ignore();
  // }
  // }

  // mapFile.close();
}
