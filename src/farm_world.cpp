#include "../include/farm_world.hpp"

#include "../include/game_assets.hpp"
#include "../include/map_loader.hpp"

#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iostream>

void FarmWorld::initialize() {
    fields.resize(GRID_HEIGHT, std::vector<Field>(GRID_WIDTH));
    updateFieldPositions();
}

void FarmWorld::updateFieldPositions() {
    const float centerX = WINDOW_WIDTH / 2.0f;
    const float centerY = WINDOW_HEIGHT / 2.0f - TILE_SIZE * 1.5f;
    const float isoTileWidth = TILE_SIZE;
    const float isoTileHeight = TILE_SIZE / 2.0f;

    fields.resize(GRID_HEIGHT, std::vector<Field>(GRID_WIDTH));

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            const float isoX = centerX + (x - y) * isoTileWidth / 2.0f;
            const float isoY = centerY + (x + y) * isoTileHeight / 2.0f;

            fields[y][x].setPosition(isoX, isoY);
            fields[y][x].setTopCenter(isoX, isoY);
            fields[y][x].setBlockType(BlockType::NONE);
            fields[y][x].setCropState(CropState::EMPTY);
            fields[y][x].setCropType(CropType::NONE);
        }
    }
}

void FarmWorld::applyTextures(const GameAssets& assets) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            const BlockType blockType = fields[y][x].getBlockType();

            switch (blockType) {
                case BlockType::GRASS_BLOCK:
                    fields[y][x].setTexture(assets.grass());
                    break;
                case BlockType::DIRT:
                    fields[y][x].setTexture(assets.dirt());
                    break;
                case BlockType::FARMLAND_DRY:
                    fields[y][x].setTexture(assets.farmlandDry());
                    break;
                case BlockType::FARMLAND_WET:
                    fields[y][x].setTexture(assets.farmlandWet());
                    break;
                case BlockType::WATER:
                    fields[y][x].setTexture(assets.water());
                    break;
                default:
                    break;
            }

            CropType cType = fields[y][x].getCropType();
            if (cType == CropType::WHEAT && fields[y][x].getCropState() != CropState::EMPTY) {
                fields[y][x].setPlantTexture(assets.wheat(fields[y][x].cropAge));
            }
            else if (cType == CropType::CARROT && fields[y][x].getCropState() != CropState::EMPTY) {
                fields[y][x].setPlantTexture(assets.carrot(fields[y][x].cropAge));
            }
        }
    }
}

void FarmWorld::createNewGame(const std::vector<sf::Texture>& blockTextures) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            fields[y][x].setBlockType(BlockType::NONE);
            fields[y][x].setCropState(CropState::EMPTY);
            fields[y][x].setCropType(CropType::NONE);
            fields[y][x].cropAge = 0;
            fields[y][x].isFocused = false;
        }
    }

    focusedFieldCoords = sf::Vector2f(-1.0f, -1.0f);

    const std::string mapFilePath = MapLoader::resolveDefaultMapPath();
    if (mapFilePath.empty()) {
        std::cerr << "No map files found in directory: " << MAPS_DIR << std::endl;
        return;
    }

    const std::vector<std::vector<int>> mapData = MapLoader::loadMapFromFile(mapFilePath);
    if (mapData.empty()) {
        std::cerr << "Failed to load default map from file: " << mapFilePath << std::endl;
        return;
    }

    MapLoader::applyMapToFields(fields, mapData, blockTextures);
    std::cout << "New game created from default map" << std::endl;
}

void FarmWorld::updateMouseFocus(float mousePosX, float mousePosY, bool mouseButtonLeftPressed, const sf::Texture& focusTexture, sf::RenderWindow& window) {
    focusedFieldCoords = sf::Vector2f(-1.0f, -1.0f);

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            fields[y][x].isFocused = false;
        }
    }

    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            const sf::Vector2f center = fields[y][x].getTopCenter();
            const bool insideIso = std::abs(mousePosX - center.x) / (TILE_SIZE / 2.0f) +
                                   std::abs(mousePosY - center.y) / (((TILE_SIZE + 16) * 0.45f) / 2.0f) <=
                                   1;
            if (!insideIso) {
                continue;
            }

            if (!mouseButtonLeftPressed) {
                fields[y][x].isFocused = true;
            }
            focusedFieldCoords = sf::Vector2f(static_cast<float>(y), static_cast<float>(x));
            fields[y][x].setFocusTexture(focusTexture, window);
        }
    }
}

bool FarmWorld::hasFocusedField() const {
    return !(focusedFieldCoords.x < 0 || focusedFieldCoords.y < 0);
}

bool FarmWorld::plant(InventoryItem* selectedItem, const GameAssets& assets) {
    if (!hasFocusedField() || selectedItem == nullptr) return false;

    CropType typeToPlant = CropType::NONE;
    if (selectedItem->id == 3) typeToPlant = CropType::WHEAT;
    else if (selectedItem->id == 4) typeToPlant = CropType::CARROT;

    if (typeToPlant == CropType::NONE) return false;

    Field& field = fields[static_cast<int>(focusedFieldCoords.x)][static_cast<int>(focusedFieldCoords.y)];

    if (field.getBlockType() != BlockType::FARMLAND_DRY && field.getBlockType() != BlockType::FARMLAND_WET) return false;
    if (field.getCropState() != CropState::EMPTY) return false;

    field.setCropType(typeToPlant);
    field.setCropState(CropState::SEED);
    field.cropAge = 0;

    if (typeToPlant == CropType::WHEAT) field.setPlantTexture(assets.wheat(0));
    if (typeToPlant == CropType::CARROT) field.setPlantTexture(assets.carrot(0));

    selectedItem->amount -= 1;
    if (selectedItem->amount < 0) selectedItem->amount = 0;
    return true;
}

bool FarmWorld::till(InventoryItem* selectedItem, const GameAssets& assets) {
    if (!hasFocusedField() || selectedItem == nullptr) return false;
    if (selectedItem->id != 1) return false;

    Field& field = fields[static_cast<int>(focusedFieldCoords.x)][static_cast<int>(focusedFieldCoords.y)];

    if (field.getBlockType() != BlockType::DIRT && field.getBlockType() != BlockType::GRASS_BLOCK) return false;

    field.setBlockType(BlockType::FARMLAND_DRY);
    field.setTexture(assets.farmlandDry());

    return true;
}

HarvestRewards FarmWorld::harvest() {
    HarvestRewards rewards;
    if (!hasFocusedField()) {
        return rewards;
    }

    Field& field = fields[static_cast<int>(focusedFieldCoords.x)][static_cast<int>(focusedFieldCoords.y)];
    if (field.getBlockType() != BlockType::FARMLAND_DRY && field.getBlockType() != BlockType::FARMLAND_WET) {
        return rewards;
    }
    if (field.getCropState() != CropState::GROWN) {
        return rewards;
    }

    CropType harvestedType = field.getCropType();

    field.setCropType(CropType::NONE);
    field.setCropState(CropState::EMPTY);
    field.cropAge = 0;

    rewards.emeralds = 1;

    if (harvestedType == CropType::WHEAT) {
        rewards.droppedItems.push_back({2, 1});
        rewards.droppedItems.push_back({3, rand() % 2 + 1});
    }
    else if (harvestedType == CropType::CARROT) {
        rewards.droppedItems.push_back({4, rand() % 4 + 2});
    }

    std::cout << focusedFieldCoords.x << ", " << focusedFieldCoords.y << " - harvested!" << std::endl;
    return rewards;
}

void FarmWorld::tick(const GameAssets& assets) {
    farmlandHydration(assets.farmlandWet());
    plantGrowth(assets);
}

void FarmWorld::farmlandHydration(const sf::Texture& farmlandWetTexture) {
    if (fields.empty()) {
        return;
    }

    const int maxY = static_cast<int>(fields.size()) - 1;
    for (int y = 0; y <= maxY; y++) {
        if (fields[y].empty()) {
            continue;
        }

        const int maxX = static_cast<int>(fields[y].size()) - 1;
        for (int x = 0; x <= maxX; x++) {
            if (fields[y][x].getBlockType() != BlockType::FARMLAND_DRY) {
                continue;
            }

            bool nearbyWater = false;
            for (int i = std::max(0, y - 4); i <= std::min(maxY, y + 4); i++) {
                if (fields[i].empty()) {
                    continue;
                }

                const int rowMaxX = static_cast<int>(fields[i].size()) - 1;
                for (int j = std::max(0, x - 4); j <= std::min(rowMaxX, x + 4); j++) {
                    if (fields[i][j].getBlockType() == BlockType::NONE) {
                        continue;
                    }

                    if (fields[i][j].getBlockType() == BlockType::WATER) {
                        nearbyWater = true;
                        break;
                    }
                }
                if (nearbyWater) {
                    break;
                }
            }

            if (nearbyWater && rand() % 100 < 20) {
                fields[y][x].setBlockType(BlockType::FARMLAND_WET);
                fields[y][x].setTexture(farmlandWetTexture);
            }
        }
    }
}

void FarmWorld::plantGrowth(const GameAssets& assets) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            if (fields[y][x].getCropState() != CropState::SEED) {
                continue;
            }
            if (rand() % 100 >= 33) {
                continue;
            }

            if (fields[y][x].cropAge < 7) {
                fields[y][x].cropAge++;

                if (fields[y][x].getCropType() == CropType::WHEAT) {
                    fields[y][x].setPlantTexture(assets.wheat(fields[y][x].cropAge));
                }
                else if (fields[y][x].getCropType() == CropType::CARROT) {
                    fields[y][x].setPlantTexture(assets.carrot(fields[y][x].cropAge));
                }
            }

            if (fields[y][x].cropAge >= 7) {
                fields[y][x].setCropState(CropState::GROWN);
            }
        }
    }
}

void FarmWorld::renderFields(sf::RenderWindow& window) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            fields[y][x].render(window);
        }
    }
}

void FarmWorld::renderFocus(sf::RenderWindow& window) {
    for (int y = 0; y < GRID_HEIGHT; y++) {
        for (int x = 0; x < GRID_WIDTH; x++) {
            fields[y][x].renderFocus(window);
        }
    }
}
