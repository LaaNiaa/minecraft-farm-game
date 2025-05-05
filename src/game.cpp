#include "../include/game.hpp"
#include "../include/map_loader.hpp"

#include <iostream>
#include <map>

Game::Game()
    : window(sf::RenderWindow(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Minecraft Farm Game")), mapFilePath("../../maps/default_map.txt") {
    window.setFramerateLimit(60);

    gameView.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    gameView.setCenter(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f));
    window.setView(gameView);
    zoomLevel = 1.0f;
    zoomStep = 1.1f;

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
    fields.resize(GRID_HEIGHT, std::vector<Field>(GRID_WIDTH));
    updateFiledPositions();
}

void Game::updateFiledPositions() {
    float centerX = WINDOW_WIDTH / 2.0f;
    float centerY = WINDOW_HEIGHT / 2.0f - TILE_SIZE * 1.5;

    float isoTileWidth = TILE_SIZE;
    float isoTileHeight = (TILE_SIZE / 2.0f);

    fields.resize(GRID_HEIGHT, std::vector<Field>(GRID_WIDTH));

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            float isoX = centerX + (x - y) * isoTileWidth / 2.0f;
            float isoY = centerY + (x + y) * isoTileHeight / 2.0f;

            fields[y][x].setPosition(isoX, isoY);
            fields[y][x].setTopCenter(isoX, isoY);
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

void Game::handleWindowResize(unsigned int width, unsigned int height) {
    gameView.setSize(sf::Vector2f(width, height));
    handleZoom(zoomLevel);
    window.setView(gameView);
}

void Game::handleZoom(float zoomLevel) {
    gameView.zoom(zoomLevel);
    window.setView(gameView);
}

void Game::mouseFocus(float mousePosX, float mousePosY) {
    sf::Vector2f center = fields[0][0].getTopCenter();

    if (std::abs(mousePosX - center.x) / ( TILE_SIZE / 2.0f) + std::abs(mousePosY - center.y) / ( ((TILE_SIZE + 16) * 0.45) / 2.0f) <= 1) {
        std::cout << "Mouse focus" << std::endl;
    }
}

void Game::processEvents() {
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }
        else if (event->is<sf::Event::Resized>()) {
            handleWindowResize(event->getIf<sf::Event::Resized>()->size.x, event->getIf<sf::Event::Resized>()->size.y);
        }
        else if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>()) {
            if (mouseWheelScrolled->delta <= 0) {
                if (zoomLevel < 1.5f) {
                    zoomLevel *= powf(zoomStep, 1);
                    handleZoom(powf(zoomStep, 1));
                }
            }
            else {
                if (zoomLevel > 0.5f) {
                    zoomLevel *= powf(zoomStep, -1);
                    handleZoom(powf(zoomStep, -1));
                }
            }
        }
        else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
            sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
            sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
            mouseFocus(worldPos.x, worldPos.y);
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