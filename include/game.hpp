#pragma once

#include <SFML/Graphics.hpp>
#include <string>

#include "const.hpp"
#include "farm_world.hpp"
#include "game_assets.hpp"
#include "inventory_manager.hpp"
#include "save_manager.hpp"

class Game {
    public:
        Game();
        void run();

    private:
        sf::Clock gameClock;
        sf::RenderWindow window;
        std::string saveFilePath;

        int emeraldCount = 0;

        sf::View gameView;
        sf::View hudView;
        float zoomLevel;
        float zoomStep;

        bool mouseButtonLeftPressed = false;
        sf::Vector2f worldLeftClickPos;
        sf::Vector2f worldRightClickPos;

        GameAssets assets;
        FarmWorld world;
        InventoryManager inventory;

        void saveGame();
        bool loadGame();
        void createNewGame();

        void processEvents();
        void handleWindowResize(unsigned int width, unsigned int height);
        void handleZoom(float zoomLevel);
        void handleViewMovement(sf::Vector2f worldPos);

        void clock();
        void render();
};