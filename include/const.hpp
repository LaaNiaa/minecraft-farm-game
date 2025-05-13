#pragma once

const int GRID_WIDTH = 10;
const int GRID_HEIGHT = 10;
const int WINDOW_WIDTH = 1920;
const int WINDOW_HEIGHT = 1080;
const int TILE_SIZE = WINDOW_WIDTH / 12.8;

enum class BlockType {
    NONE,
    GRASS_BLOCK,
    DIRT,
    FARMLAND,
    WATER
};

enum class CropState {
    EMPTY
};