#include "../include/game.hpp"
#include "../include/map_loader.hpp"

#include <iostream>
#include <map>

Game::Game()
    : window(sf::RenderWindow(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Minecraft Farm Game")), mapFilePath("../../maps/default_map.txt") {
    window.setFramerateLimit(60);

    loadTextures();
    initializeFields();
    loadMap(mapFilePath);
}

void Game::loadTextures() {
    if (!textures.Grass_Block.loadFromFile("../../textures/Grass_Block.png")) {
        std::cerr << "Failed to load Grass_Block texture" << std::endl;
    }

    blockTextures.push_back(textures.Grass_Block);
}

void Game::initializeFields() {
    float centerX = WINDOW_WIDTH / 2.0f;
    float centerY = WINDOW_HEIGHT / 2.0f - TILE_SIZE * 1.5;

    float isoTileWidth = TILE_SIZE - 10;
    float isoTileHeight = (TILE_SIZE / 2.0f) - 10;

    fields.resize(GRID_HEIGHT, std::vector<Field>(GRID_WIDTH));

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            float isoX = centerX + (x - y) * isoTileWidth / 2.0f;
            float isoY = centerY + (x + y) * isoTileHeight / 2.0f;

            fields[y][x].setPosition(isoX, isoY);
            fields[y][x].setBlockType(BlockType::NONE);
            fields[y][x].setCropState(CropState::EMPTY);
        }
    }
}

void Game::loadMap(const std::string& filename) {
    std::vector<std::vector<int>> mapData = MapLoader::loadMapFromFile(filename);

    if (mapData.empty()) {
        std::cerr << "Failed to load map from file: " << filename << std::endl;
    }

    MapLoader::applyMapToFields(fields, mapData, blockTextures);
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        render();
    }
}

void Game::processEvents() {
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }
    }
}

void Game::render() {
    window.clear();

    renderFields();

    window.display();
}

void Game::renderFields() {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            fields[y][x].render(window);
        }
    }
}