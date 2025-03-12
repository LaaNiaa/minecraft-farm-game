#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>

#include "const.hpp"
#include "field.hpp"

class Game {
    public:
        Game();
        void run();

    private:
        sf::RenderWindow window;
        std::vector<std::vector<Field>> fields;
        std::string mapFilePath;

        struct Textures {
            sf::Texture Grass_Block;
        } textures;

    std::vector<sf::Texture> blockTextures;

        void loadTextures();
        void initializeFields();
        void loadMap(const std::string& filename);

        void processEvents();

        void render();
        void renderFields();
};