#include "../include/map_loader.hpp"
#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>

std::vector<MapFileInfo> MapLoader::listMapFiles(const std::string& directory) {
    std::vector<MapFileInfo> mapFiles;

    std::error_code errorCode;
    const std::filesystem::path directoryPath(directory);
    if (!std::filesystem::exists(directoryPath, errorCode) || !std::filesystem::is_directory(directoryPath, errorCode)) {
        if (errorCode) {
            std::cerr << "Failed to access map directory: " << directory
                      << " (" << errorCode.message() << ")" << std::endl;
        }
        return mapFiles;
    }

    try {
        for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
            if (!entry.is_regular_file()) {
                continue;
            }

            const std::filesystem::path filePath = entry.path();
            if (filePath.extension() != ".txt") {
                continue;
            }

            MapFileInfo mapInfo;
            mapInfo.path = filePath.string();
            mapInfo.filename = filePath.filename().string();
            mapFiles.push_back(mapInfo);
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Failed to list map files in: " << directory << " (" << e.what() << ")" << std::endl;
        return {};
    }

    std::sort(mapFiles.begin(), mapFiles.end(), [](const MapFileInfo& left, const MapFileInfo& right) {
        return left.filename < right.filename;
    });

    return mapFiles;
}

std::string MapLoader::resolveDefaultMapPath(const std::string& directory) {
    const std::filesystem::path defaultMapPath = std::filesystem::path(directory) / DEFAULT_MAP_FILE;
    if (std::filesystem::exists(defaultMapPath)) {
        return defaultMapPath.string();
    }

    const std::vector<MapFileInfo> mapFiles = listMapFiles(directory);
    if (!mapFiles.empty()) {
        return mapFiles.front().path;
    }

    return "";
}

std::vector<std::vector<int>> MapLoader::loadMapFromFile(const std::string& filename) {
    std::vector<std::vector<int>> mapData;

    if (!std::filesystem::exists(filename)) {
        std::cerr << "Map file does not exist: " << filename << std::endl;
        return mapData;
    }

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

void MapLoader::applyMapToFields(std::vector<std::vector<Field>>& fields, const std::vector<std::vector<int>>& mapData, const std::vector<sf::Texture>& textures) {
  int height = std::min(static_cast<int>(mapData.size()), GRID_HEIGHT);

  for (int y = 0; y < height; y++) {
    int width = std::min(static_cast<int>(mapData[y].size()), GRID_WIDTH);

    for (int x = 0; x < width; x++) {
      int blockTypeInt = mapData[y][x];
      BlockType blockType = intToBlockType(blockTypeInt);

      fields[y][x].setBlockType(blockType);
      fields[y][x].cropAge = 0;

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
      return BlockType::FARMLAND_DRY;
      break;
    case 4:
      return BlockType::FARMLAND_WET;
      break;
    case 5:
      return BlockType::WATER;
    break;
    default:
      return BlockType::NONE;
      break;
  }
}