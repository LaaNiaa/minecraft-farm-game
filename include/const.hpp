#pragma once

#include <string>

const int GRID_WIDTH = 10;
const int GRID_HEIGHT = 10;
const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;
const int TILE_SIZE = WINDOW_WIDTH / 12.8;

inline const std::string SAVES_DIR = "../../saves";
inline const std::string MAPS_DIR = "../../maps";
inline const std::string DEFAULT_SAVE_FILE = "game_save.json";
inline const std::string DEFAULT_MAP_FILE = "default_map.txt";

enum class BlockType {
    NONE,
    GRASS_BLOCK,
    DIRT,
    FARMLAND_DRY,
    FARMLAND_WET,
    WATER
};

enum class CropState {
    EMPTY,
    SEED,
    GROWN
};

enum class CropType {
    NONE,
    WHEAT
};