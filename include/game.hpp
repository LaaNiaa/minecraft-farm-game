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

        sf::FloatRect chestBound;
        sf::FloatRect inventoryBound;
        sf::FloatRect hotbarBound;
        bool inventoryOpened = false;
        const float inventorySlotSize = 16.0f;
        const float inventorySpacing = 2.0f;
        const float inventoryOffset = 11.0f;
        const int itemStack = 64;

        struct InventoryItem {
            int id = 0;
            int amount = 0;
        };

        std::array<sf::FloatRect, 27> inventory;
        std::array<int, 27> inventoryStacks;
        std::array<InventoryItem, 27> inventoryItems = {};

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
            sf::Texture Inventory;
            sf::Texture Hotbar;

            sf::Texture Wheat_Item;
            sf::Texture Wheat_Seeds_Item;
        } textures;

        sf::Texture itemTexture;

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
        void renderInventory();
        void addItemToInventory(int itemID, int amount);
};