#include "../include/map_loader.hpp"
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<std::vector<int>> MapLoader::loadMapFromFile(const std::string& filename) {
    std::vector<std::vector<int>> mapData;
    std::ifstream file(filename);

    if (!file.is_open()) {
      std::cerr << "Failed to open map file " << filename << std::endl;
      file.close();
      return mapData;
    }

    std::string line;
    while (std::getline(file, line)) {
      std::vector<int> row;
      std::stringstream ss(line);
      std::string cell;

      while (std::getline(ss, cell, '|')) {
        try {
          int blockType = std::stoi(cell);
          row.push_back(blockType);
        } catch (const std::exception& e) {
          row.push_back(0);
        }
      }

      if (!row.empty()) {
        mapData.push_back(row);
      }
    }

    file.close();
    return mapData;
}

void MapLoader::applyMapToFields(std::vector<std::vector<Field>>& fields,
                                 const std::vector<std::vector<int>>& mapData,
                                 const std::vector<sf::Texture>& textures) {
  int height = std::min(static_cast<int>(mapData.size()), GRID_HEIGHT);

  for (int y = 0; y < height; y++) {
    int width = std::min(static_cast<int>(mapData[y].size()), GRID_WIDTH);

    for (int x = 0; x < width; x++) {
      int blockTypeInt = mapData[y][x];
      BlockType blockType = intToBlockType(blockTypeInt);

      fields[y][x].setBlockType(blockType);

      if (blockType != BlockType::NONE && blockTypeInt > 0 && blockTypeInt <= textures.size()) {
        fields[y][x].setTexture(textures[blockTypeInt - 1]);
      }
    }
  }
}

BlockType MapLoader::intToBlockType(int value) {
  switch (value) {
    case 1:
      return BlockType::GRASS_BLOCK;
      break;
    case 2:
      return BlockType::DIRT;
      break;
    case 3:
      return BlockType::FARMLAND;
      break;
    case 4:
      return BlockType::WATER;
      break;
    default:
      return BlockType::NONE;
      break;
  }
}