#include "../include/game.hpp"

#include <iostream>

Game::Game()
    : window(sf::RenderWindow(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Minecraft Farm Game")) {
    window.setFramerateLimit(60);

    loadTextures();
    initializeFields();
}

void Game::loadTextures() {
    if (!textures.Grass_Block.loadFromFile("../../textures/Grass_Block.png")) {
        std::cerr << "Failed to load Grass_Block texture" << std::endl;
    }
}

void Game::initializeFields() {
    float startX = (WINDOW_WIDTH - GRID_WIDTH * TILE_SIZE) / 2.0f;
    float startY = (WINDOW_HEIGHT - GRID_HEIGHT * TILE_SIZE) / 2.0f;

    fields.resize(GRID_HEIGHT, std::vector<Field>(GRID_WIDTH));

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            fields[y][x].setPosition(startX + x * TILE_SIZE, startY + y * TILE_SIZE);
            fields[y][x].setBlockType(BlockType::GRASS_BLOCK);
            fields[y][x].setTexture(textures.Grass_Block);
        }
    }
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