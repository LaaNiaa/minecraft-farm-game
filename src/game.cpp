#include "../include/game.hpp"
#include "../include/map_loader.hpp"

#include <iostream>

Game::Game()
    : window(sf::RenderWindow(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Minecraft Farm Game")), mapFilePath("../../maps/default_map.txt") {
    window.setFramerateLimit(60);

    gameView.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    gameView.setCenter(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f));
    window.setView(gameView);
    zoomLevel = 1.0f;
    zoomStep = 1.1f;

    hudView.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    hudView.setCenter(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f));

    loadTextures();
    initializeFields();
    loadMap(mapFilePath);

    //load empty inventory for now
    for (int i = 0; i < 27; i++) {
        if (inventoryItems[i].id == 0) {
            inventoryItems[i].id = 0;
            inventoryItems[i].amount = 0;
        }
    }

    addItemToInventory(2, 10);
}

void Game::loadTextures() {
    if (!textures.Grass_Block.loadFromFile("../../textures/Grass_Block.png")) {
        std::cerr << "Failed to load Grass_Block texture" << std::endl;
    }

    if (!textures.Focused.loadFromFile("../../textures/Focused.png")) {
        std::cerr << "Failed to load Focused texture" << std::endl;
    }

    if (!textures.Dirt.loadFromFile("../../textures/Dirt.png")) {
        std::cerr << "Failed to load Dirt texture" << std::endl;
    }

    if (!textures.Farmland_Dry.loadFromFile("../../textures/Farmland_Dry.png")) {
        std::cerr << "Failed to load Farmland_Dry texture" << std::endl;
    }

    if (!textures.Farmland_Wet.loadFromFile("../../textures/Farmland_Wet.png")) {
        std::cerr << "Failed to load Farmland_Wet texture" << std::endl;
    }

    if (!textures.Water.loadFromFile("../../textures/Water.png")) {
        std::cerr << "Failed to load Water texture" << std::endl;
    }

    blockTextures.push_back(textures.Grass_Block);
    blockTextures.push_back(textures.Dirt);
    blockTextures.push_back(textures.Farmland_Dry);
    blockTextures.push_back(textures.Farmland_Wet);
    blockTextures.push_back(textures.Water);

    if (!textures.Wheat_Age_0.loadFromFile("../../textures/wheat/Wheat_Age_0.png")) {
        std::cerr << "Failed to load Wheat_Age_0 texture" << std::endl;
    }

    if (!textures.Wheat_Age_1.loadFromFile("../../textures/wheat/Wheat_Age_1.png")) {
        std::cerr << "Failed to load Wheat_Age_1 texture" << std::endl;
    }

    if (!textures.Wheat_Age_2.loadFromFile("../../textures/wheat/Wheat_Age_2.png")) {
        std::cerr << "Failed to load Wheat_Age_2 texture" << std::endl;
    }

    if (!textures.Wheat_Age_3.loadFromFile("../../textures/wheat/Wheat_Age_3.png")) {
        std::cerr << "Failed to load Wheat_Age_3 texture" << std::endl;
    }

    if (!textures.Wheat_Age_4.loadFromFile("../../textures/wheat/Wheat_Age_4.png")) {
        std::cerr << "Failed to load Wheat_Age_4 texture" << std::endl;
    }

    if (!textures.Wheat_Age_5.loadFromFile("../../textures/wheat/Wheat_Age_5.png")) {
        std::cerr << "Failed to load Wheat_Age_5 texture" << std::endl;
    }

    if (!textures.Wheat_Age_6.loadFromFile("../../textures/wheat/Wheat_Age_6.png")) {
        std::cerr << "Failed to load Wheat_Age_6 texture" << std::endl;
    }

    if (!textures.Wheat_Age_7.loadFromFile("../../textures/wheat/Wheat_Age_7.png")) {
        std::cerr << "Failed to load Wheat_Age_7 texture" << std::endl;
    }


    if (!textures.Emerald.loadFromFile("../../textures/hud/Emerald.png")) {
        std::cerr << "Failed to load Emerald texture" << std::endl;
    }

    if (!textures.Chest.loadFromFile("../../textures/hud/Chest.png")) {
        std::cerr << "Failed to load Chest texture" << std::endl;
    }

    if (!textures.Inventory.loadFromFile("../../textures/hud/Inventory.png")) {
        std::cerr << "Failed to load Inventory texture" << std::endl;
    }

    if (!textures.Hotbar.loadFromFile("../../textures/hud/Hotbar.png")) {
        std::cerr << "Failed to load Hotbar texture" << std::endl;
    }

    if (!textures.Wheat_Item.loadFromFile("../../textures/items/Wheat_Item.png")) {
        std::cerr << "Failed to load Wheat_Item texture" << std::endl;
    }

    if (!textures.Wheat_Seeds_Item.loadFromFile("../../textures/items/Wheat_Seeds_Item.png")) {
        std::cerr << "Failed to load Wheat_Seeds_Item texture" << std::endl;
    }

    if (!font.openFromFile("../../textures/font/Minecraft.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
    }
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
            fields[y][x].setCropType(CropType::NONE);

            //std::cout << "Field: " << y << x << " : x " << isoX << ", y " << isoY << " id: " << id << std::endl;
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
        clock();
        processEvents();
        render();
    }
}

void Game::handleWindowResize(unsigned int width, unsigned int height) {
    gameView.setSize(sf::Vector2f(width, height));
    handleZoom(zoomLevel);
    window.setView(gameView);

    hudView.setSize(sf::Vector2f(width, height));
    hudView.setCenter(sf::Vector2f(width / 2.0f, height / 2.0f));
}

void Game::handleZoom(float zoomLevel) {
    gameView.zoom(zoomLevel);
    window.setView(gameView);
}

void Game::handleViewMovement(sf::Vector2f worldPos) {
    gameView.move(worldLeftClickPos - worldPos);
    window.setView(gameView);
}


void Game::mouseFocus(float mousePosX, float mousePosY) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            fields[y][x].isFocused = false;
            focusedField = sf::Vector2f(-1.0f, -1.0f);
        }
    }

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            sf::Vector2f center = fields[y][x].getTopCenter();

            if (std::abs(mousePosX - center.x) / ( TILE_SIZE / 2.0f) + std::abs(mousePosY - center.y) / ( ((TILE_SIZE + 16) * 0.45) / 2.0f) <= 1) {
                if (mouseButtonLeftPressed == false) {
                    fields[y][x].isFocused = true;
                }
                focusedField = sf::Vector2f(y, x);
                fields[y][x].setFocusTexture(textures.Focused, window);
        }
    }

    //std::cout << focusedField.x << focusedField.y << std::endl;
    //std::cout << center.x << ", " << center.y << std::endl;
    //std::cout << "Mouse focus" << std::endl;
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
            if(inventoryOpened == false) {
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
        }
        else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
            sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
            sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
            mouseFocus(worldPos.x, worldPos.y);

            if (mouseButtonLeftPressed == true && inventoryOpened == false) {
                handleViewMovement(worldPos);
            }

            //std::cout << "mouse x: " << worldPos.x << std::endl;
            //std::cout << "mouse y: " << worldPos.y << std::endl;
        }
        else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                sf::Vector2i pixelLeftClickPos = mouseButtonPressed->position;
                worldLeftClickPos = window.mapPixelToCoords(pixelLeftClickPos);

                mouseButtonLeftPressed = true;

                sf::Vector2f hudMousePos = window.mapPixelToCoords(pixelLeftClickPos, hudView);

                if (inventoryOpened == true) {
                    for (int i = 0; i < 27; i++) {
                        if (inventory[i].contains(hudMousePos)) {
                            std::cout << "Clicked slot: " << i << std::endl;
                            std::cout << "Item: " << inventoryItems[i].id << " - amount: " << inventoryItems[i].amount << std::endl;

                            if (inventoryItems[i].id != 0) {
                                selectedItem = &inventoryItems[i];
                                inventoryOpened = false;
                            }

                            return;
                        }
                    }
                }

                if (chestBound.contains(hudMousePos) && inventoryOpened == false) {
                    std::cout << "Chest clicked" << std::endl;
                    inventoryOpened = true;
                    return;
                }

                if (inventoryOpened == true && inventoryBound.contains(hudMousePos)) {
                    return;
                }

                if (inventoryOpened == true && !inventoryBound.contains(hudMousePos)) {
                    std::cout << "Inventory closed" << std::endl;
                    inventoryOpened = false;
                    return;
                }

                std::cout << "mouse: " << worldLeftClickPos.x << ", " << worldLeftClickPos.y << " | field: " << focusedField.x << ", " << focusedField.y << std::endl;
            }
            else if (mouseButtonPressed->button == sf::Mouse::Button::Right) {
                sf::Vector2i pixelRightClickPos = mouseButtonPressed->position;
                worldRightClickPos = window.mapPixelToCoords(pixelRightClickPos);

                if (inventoryOpened == true) {
                    return;
                }

                if(inventoryOpened == false) {
                    if (focusedField.x == -1 && focusedField.y == -1) {
                        std::cout << "mouse right: " << worldRightClickPos.x << ", " << worldRightClickPos.y << " | field: outside the map" << std::endl;
                        return;
                    }

                    plant();
                    harvest();
                }

                std::cout << "mouse right: " << worldRightClickPos.x << ", " << worldRightClickPos.y << " | field: " << focusedField.x << ", " << focusedField.y << std::endl;
            }
        }
        else if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
            if (mouseButtonReleased->button == sf::Mouse::Button::Left) {
                mouseButtonLeftPressed = false;
            }
        }
    }
}

void Game::plant() {
    if (fields[focusedField.x][focusedField.y].getBlockType() == BlockType::FARMLAND_DRY || fields[focusedField.x][focusedField.y].getBlockType() == BlockType::FARMLAND_WET) {
        if (fields[focusedField.x][focusedField.y].getCropState() == CropState::EMPTY) {
            if (selectedItem != nullptr && selectedItem->id == 2) {
                fields[focusedField.x][focusedField.y].setCropType(CropType::WHEAT);
                fields[focusedField.x][focusedField.y].setCropState(CropState::SEED);
                fields[focusedField.x][focusedField.y].setPlantTexture(textures.Wheat_Age_0);
                fields[focusedField.x][focusedField.y].cropAge = 0;
                selectedItem->amount -= 1;
                if (selectedItem->amount <= 0) {
                    selectedItem->id = 0;
                    selectedItem->amount = 0;
                    selectedItem = nullptr;
                }
            }
        }
    }
}

void Game::harvest() {
    if (fields[focusedField.x][focusedField.y].getBlockType() == BlockType::FARMLAND_DRY || fields[focusedField.x][focusedField.y].getBlockType() == BlockType::FARMLAND_WET) {
        if (fields[focusedField.x][focusedField.y].getCropState() == CropState::GROWN) {
            fields[focusedField.x][focusedField.y].setCropType(CropType::NONE);
            fields[focusedField.x][focusedField.y].setCropState(CropState::EMPTY);
            //fields[focusedField.x][focusedField.y].setPlantTexture(textures.Wheat_Age_1);
            fields[focusedField.x][focusedField.y].cropAge = 0;

            emeraldCount++;
            addItemToInventory(1, 1);
            addItemToInventory(2, 1);

            std::cout << focusedField.x << ", " << focusedField.y << " - harvested!" << std::endl;
        }
    }
}

void Game::plantGrowth() {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (fields[y][x].getCropState() == CropState::SEED) {
                switch (fields[y][x].cropAge) {
                    case 0:
                        if (rand()%100 < 33) {
                            fields[y][x].cropAge++;
                            fields[y][x].setPlantTexture(textures.Wheat_Age_1);
                        }
                    break;
                    case 1:
                        if (rand()%100 < 33) {
                            fields[y][x].cropAge++;
                            fields[y][x].setPlantTexture(textures.Wheat_Age_2);
                        }
                    break;
                    case 2:
                        if (rand()%100 < 33) {
                            fields[y][x].cropAge++;
                            fields[y][x].setPlantTexture(textures.Wheat_Age_3);
                        }
                    break;
                    case 3:
                        if (rand()%100 < 33) {
                            fields[y][x].cropAge++;
                            fields[y][x].setPlantTexture(textures.Wheat_Age_4);
                        }
                    break;
                    case 4:
                        if (rand()%100 < 33) {
                            fields[y][x].cropAge++;
                            fields[y][x].setPlantTexture(textures.Wheat_Age_5);
                        }
                    break;
                    case 5:
                        if (rand()%100 < 33) {
                            fields[y][x].cropAge++;
                            fields[y][x].setPlantTexture(textures.Wheat_Age_6);
                        }
                    break;
                    case 6:
                        if (rand()%100 < 33) {
                            fields[y][x].cropAge++;
                            fields[y][x].setPlantTexture(textures.Wheat_Age_7);
                            fields[y][x].setCropState(CropState::GROWN);
                        }
                    break;
                    default:
                        std::cout << "crop age is not recognized" << std::endl;
                    break;
                }
            }
        }
    }
}

void Game::clock() {
    sf::Time gameTime = gameClock.getElapsedTime();

    if (gameTime.asMilliseconds() >= 1000) {
        gameClock.restart();
        std::cout << "Tick" << std::endl;

        farmlandHydration();
        plantGrowth();
    }
}

void Game::farmlandHydration() {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (fields[y][x].getBlockType() == BlockType::FARMLAND_DRY) {
                for (int i = y-4; i <= y+4; i++) {
                    if (flag == true) {
                        flag = false;
                        break;
                    }
                    for (int j = x-4; j <= x+4; j++) {
                        if (fields[i][j].getBlockType() == BlockType::WATER) {
                            std::cout << "WATER" << " | " << y << " " << x << " | " << i << " " << j << std::endl;
                            if (rand()%100 < 20) {
                                //does not change the map yet
                                fields[y][x].setBlockType(BlockType::FARMLAND_WET);
                                fields[y][x].setTexture(textures.Farmland_Wet);
                            }
                            flag = true;
                            break;
                        }
                    }
                }
            }
        }
    }
}


void Game::render() {
    window.clear(sf::Color(110, 177, 255));

    renderFields();

    if (inventoryOpened == false) {
        renderFocusFields();
    }

    window.setView(hudView);
    renderHud();
    if (inventoryOpened == true) {
        renderInventory();
    }

    window.setView(gameView);

    window.display();
}

void Game::renderFields() {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            fields[y][x].render(window);
        }
    }
}

void Game::renderFocusFields() {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            fields[y][x].renderFocus(window);
        }
    }
}

void Game::renderHud() {
    const float margin = 30.0f;
    const float iconSize = 48.0f;

    sf::Sprite emeraldSprite(textures.Emerald);

    sf::Vector2u texSize = textures.Emerald.getSize();
    float scaleX = iconSize / static_cast<float>(texSize.x);
    float scaleY = iconSize / static_cast<float>(texSize.y);
    emeraldSprite.setScale(sf::Vector2f(scaleX, scaleY));
    emeraldSprite.setPosition(sf::Vector2f(margin - (30 * scaleX), margin - (20 * scaleY)));

    window.draw(emeraldSprite);


    sf::Text countText(font, std::to_string(emeraldCount), 32);
    countText.setFillColor(sf::Color::White);
    countText.setOutlineColor(sf::Color::Black);
    countText.setOutlineThickness(2.0f);

    countText.setPosition(sf::Vector2f(margin + iconSize + 10.0f, margin + (iconSize * 0.1f) - (20 * scaleY)));

    window.draw(countText);

    const float chestIconSize = 150.0f;
    sf::Sprite chestSprite(textures.Chest);

    sf::Vector2u chestTexSize = textures.Chest.getSize();
    float chestScaleX = chestIconSize / static_cast<float>(chestTexSize.x);
    float chestScaleY = chestIconSize / static_cast<float>(chestTexSize.y);
    chestSprite.setScale(sf::Vector2f(chestScaleX, chestScaleY));
    chestSprite.setPosition(sf::Vector2f(margin - (42 * chestScaleX), hudView.getSize().y - chestIconSize - margin - (3 * chestScaleY)));
    chestBound = chestSprite.getGlobalBounds();

    window.draw(chestSprite);

    const float hotbarSize = 100.0f;
    sf::Sprite hotbarSprite(textures.Hotbar);

    sf::Vector2u hotbarTexSize = textures.Hotbar.getSize();
    float hotbarScaleX = hotbarSize / static_cast<float>(hotbarTexSize.x);
    float hotbarScaleY = hotbarSize / static_cast<float>(hotbarTexSize.y);
    hotbarSprite.setScale(sf::Vector2f(hotbarScaleX, hotbarScaleY));
    hotbarSprite.setPosition(sf::Vector2f(hudView.getSize().x - hotbarSize - margin, hudView.getSize().y - hotbarSize - margin));
    hotbarBound = hotbarSprite.getGlobalBounds();

    window.draw(hotbarSprite);

    if (selectedItem != nullptr) {
        sf::Sprite selectedItemSprite(itemTexture);

        selectedItemSprite.setPosition(sf::Vector2f(hudView.getSize().x - hotbarSize - margin + (4 * hotbarScaleX), hudView.getSize().y - hotbarSize - margin + (4 * hotbarScaleX)));
        float selectedItemScale = (hotbarSize - (8 * hotbarScaleX)) / 160;
        selectedItemSprite.setScale(sf::Vector2f(selectedItemScale, selectedItemScale));

        if (selectedItem->id == 1) {
            itemTexture = textures.Wheat_Item;
            window.draw(selectedItemSprite);
        }

        if (selectedItem->id == 2) {
            itemTexture = textures.Wheat_Seeds_Item;
            window.draw(selectedItemSprite);
        }

        sf::Text selectedItemCountText(font, std::to_string(selectedItem->amount), 32);
        selectedItemCountText.setFillColor(sf::Color::White);
        selectedItemCountText.setOutlineColor(sf::Color::Black);
        selectedItemCountText.setOutlineThickness(2.0f);

        if (selectedItem->amount > 9) {
            selectedItemCountText.setPosition(sf::Vector2f(hudView.getSize().x - hotbarSize - margin + (4 * hotbarScaleX) + 32, hudView.getSize().y - hotbarSize - margin + (4 * hotbarScaleX) + 32));
        }

        if (selectedItem->amount <= 9 && selectedItem->amount > 0) {
            selectedItemCountText.setPosition(sf::Vector2f(hudView.getSize().x - hotbarSize - margin + (4 * hotbarScaleX) + 48, hudView.getSize().y - hotbarSize - margin + (4 * hotbarScaleX) + 32));
        }

        window.draw(selectedItemCountText);
    }
}

void Game::renderInventory() {
    const float inventorySize = (hudView.getSize().x * 0.65);
    sf::Sprite inventorySprite(textures.Inventory);

    sf::Vector2u inventoryTexSize = textures.Inventory.getSize();
    float inventoryScale = inventorySize / static_cast<float>(inventoryTexSize.x);
    inventorySprite.setScale(sf::Vector2f(inventoryScale, inventoryScale));
    inventorySprite.setPosition(sf::Vector2f(hudView.getCenter().x - (inventoryTexSize.x * inventoryScale)/2  - (3 * inventoryScale), hudView.getCenter().y - (inventoryTexSize.y * inventoryScale)/2 - (3 * inventoryScale)));
    inventoryBound = inventorySprite.getGlobalBounds();

    sf::Vector2f inventoryPos = inventorySprite.getPosition();

    window.draw(inventorySprite);

    sf::Sprite itemSprite(itemTexture);
    float itemScale = (inventorySlotSize * inventoryScale) / 160;
    itemSprite.setScale(sf::Vector2f(itemScale, itemScale));

    int slotIndex = 0;
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 9; col++) {
            float slotX = inventoryPos.x + (inventoryOffset + col * (inventorySlotSize + inventorySpacing)) * inventoryScale;
            float slotY = inventoryPos.y + (inventoryOffset + row * (inventorySlotSize + inventorySpacing)) * inventoryScale;

            inventory[slotIndex] = sf::FloatRect(
                sf::Vector2f(slotX, slotY),
                sf::Vector2f(inventorySlotSize * inventoryScale, inventorySlotSize * inventoryScale)
            );

            itemSprite.setPosition(sf::Vector2f(slotX, slotY));

            if (inventoryItems[slotIndex].id == 1) {
                itemTexture = textures.Wheat_Item;
                window.draw(itemSprite);
            }

            if (inventoryItems[slotIndex].id == 2) {
                itemTexture = textures.Wheat_Seeds_Item;
                window.draw(itemSprite);
            }

            sf::Text itemCountText(font, std::to_string(inventoryItems[slotIndex].amount), 32);
            itemCountText.setFillColor(sf::Color::White);
            itemCountText.setOutlineColor(sf::Color::Black);
            itemCountText.setOutlineThickness(2.0f);

            if (inventoryItems[slotIndex].amount > 9) {
                itemCountText.setPosition(sf::Vector2f(slotX + (inventorySlotSize * inventoryScale) - 32 - ((inventorySpacing * inventoryScale) / 2), slotY + (inventorySlotSize * inventoryScale) - 32 - ((inventorySpacing * inventoryScale) / 2)));
            }

            if (inventoryItems[slotIndex].amount <= 9 && inventoryItems[slotIndex].amount > 0) {
                itemCountText.setPosition(sf::Vector2f(slotX + (inventorySlotSize * inventoryScale) - 16 - ((inventorySpacing * inventoryScale) / 2), slotY + (inventorySlotSize * inventoryScale) - 32 - ((inventorySpacing * inventoryScale) / 2)));
            }

            window.draw(itemCountText);

            slotIndex++;
        }
    }
}

void Game::addItemToInventory(int itemID, int amount) {
    int remaining = amount;

    for (int i = 0; i < inventory.size() && remaining > 0; i++) {
        if (inventoryItems[i].id == itemID && inventoryItems[i].amount < itemStack) {
            int spaceInSlot = itemStack - inventoryItems[i].amount;
            int toAdd = std::min(remaining, spaceInSlot);

            inventoryItems[i].amount += toAdd;
            remaining -= toAdd;
            std::cout << "Added " << toAdd << " items to slot " << i << std::endl;
        }
    }
    for (int i = 0; i < inventory.size() && remaining > 0; i++) {
        if (inventoryItems[i].id == 0) {
            int toAdd = std::min(remaining, itemStack);

            inventoryItems[i].id = itemID;
            inventoryItems[i].amount = toAdd;
            remaining -= toAdd;
            std::cout << "Added " << toAdd << " items to new slot " << i << std::endl;
        }
    }

    if (remaining > 0) {
        std::cout << "Inventory full! Could not add " << remaining << " items." << std::endl;
        // add full inventory support!!!!
    }
}