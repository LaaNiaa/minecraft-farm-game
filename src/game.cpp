#include "../include/game.hpp"

#include <iostream>

Game::Game()
    : window(sf::RenderWindow(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Minecraft Farm Game")) {
    window.setFramerateLimit(60);

    loadTextures();
}

void Game::loadTextures() {
    if (!textures.Grass_Block.loadFromFile("../../textures/Grass_Block.png")) {
        std::cerr << "Failed to load Grass_Block texture" << std::endl;
    }
}

void Game::run() {
    while (window.isOpen()) {
        processEvents();
        render();

        sf::sleep(sf::seconds(10));
    }
}

void Game::processEvents() {
    std::optional<sf::Event> event;
    while (window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
    }
}

void Game::render() {
    window.clear();

    sf::Sprite sprite(textures.Grass_Block);
    window.draw(sprite);

    window.display();
}
