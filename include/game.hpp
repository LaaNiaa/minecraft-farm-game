#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <cmath>

#include "const.hpp"
#include "field.hpp"

class Game {
    public:
        Game();
        void run();

    private:
        sf::Clock gameClock;
        sf::RenderWindow window;
        std::vector<std::vector<Field>> fields;
        std::string mapFilePath;
        sf::View gameView;
        float zoomLevel;
        float zoomStep;
        sf::Vector2f focusedField;
        bool mouseButtonLeftPressed = false;
        sf::Vector2f worldLeftClickPos;
        bool flag = false;

        struct Textures {
            sf::Texture Focused;
            sf::Texture Grass_Block;
            sf::Texture Dirt;
            sf::Texture Farmland_Dry;
            sf::Texture Farmland_Wet;
            sf::Texture Water;
        } textures;

    std::vector<sf::Texture> blockTextures;

        void loadTextures();
        void initializeFields();
        void loadMap(const std::string& filename);
        void updateFiledPositions();

        void processEvents();
        void handleWindowResize(unsigned int width, unsigned int height);
        void handleZoom(float zoomLevel);
        void handleViewMovement(sf::Vector2f worldPos);
        void mouseFocus(float mousePosX, float mousePosY);

        void farmlandHydration();

        void clock();
        void render();
        void renderFields();
};