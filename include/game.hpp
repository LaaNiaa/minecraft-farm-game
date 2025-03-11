#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "const.hpp"
#include "field.hpp"

class Game {
    public:
        Game();
        void run();

    private:
        sf::RenderWindow window;

        struct Textures {
            sf::Texture Grass_Block;
        } textures;

        void loadTextures();
        void initializeFields();

        void processEvents();

        void render();
        void renderFields();
};