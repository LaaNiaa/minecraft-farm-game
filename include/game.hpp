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

        int emeraldCount = 0;

    private:
        sf::Clock gameClock;
        sf::RenderWindow window;
        std::vector<std::vector<Field>> fields;
        std::string mapFilePath;
        sf::View gameView;
        sf::View hudView;
        float zoomLevel;
        float zoomStep;
        sf::Vector2f focusedField;
        bool mouseButtonLeftPressed = false;
        sf::Vector2f worldLeftClickPos;
        sf::Vector2f worldRightClickPos;
        bool flag = false;

        sf::Font font;

        struct Textures {
            sf::Texture Focused;
            sf::Texture Grass_Block;
            sf::Texture Dirt;
            sf::Texture Farmland_Dry;
            sf::Texture Farmland_Wet;
            sf::Texture Water;

            sf::Texture Wheat_Age_0;
            sf::Texture Wheat_Age_1;
            sf::Texture Wheat_Age_2;
            sf::Texture Wheat_Age_3;
            sf::Texture Wheat_Age_4;
            sf::Texture Wheat_Age_5;
            sf::Texture Wheat_Age_6;
            sf::Texture Wheat_Age_7;

            sf::Texture Emerald;
            sf::Texture Chest;
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
        void plant();
        void plantGrowth();
        void harvest();

        void clock();
        void render();
        void renderFields();
        void renderFocusFields();
        void renderHud();
};