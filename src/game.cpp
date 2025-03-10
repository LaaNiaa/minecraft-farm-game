#include "../include/game.hpp"

Game::Game()
    : window(sf::RenderWindow(sf::VideoMode({1920u, 1080u}), "Minecraft Farm Game")) {
    window.setFramerateLimit(60);

    while (window.isOpen())
    {
        while (const std::optional<sf::Event> event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        window.clear();
        window.display();
    }
}