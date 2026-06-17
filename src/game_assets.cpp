#include "../include/game_assets.hpp"

#include <iostream>

bool GameAssets::loadTexture(sf::Texture& texture, const std::string& path, const std::string& label) const {
    if (!texture.loadFromFile(path)) {
        std::cerr << "Failed to load " << label << " texture" << std::endl;
        return false;
    }
    return true;
}

bool GameAssets::loadAll() {
    bool success = true;

    success &= loadTexture(grassBlockTexture, "../../textures/Grass_Block.png", "Grass_Block");
    success &= loadTexture(focusedTexture, "../../textures/Focused.png", "Focused");
    success &= loadTexture(dirtTexture, "../../textures/Dirt.png", "Dirt");
    success &= loadTexture(farmlandDryTexture, "../../textures/Farmland_Dry.png", "Farmland_Dry");
    success &= loadTexture(farmlandWetTexture, "../../textures/Farmland_Wet.png", "Farmland_Wet");
    success &= loadTexture(waterTexture, "../../textures/Water.png", "Water");

    success &= loadTexture(wheatTextures[0], "../../textures/wheat/Wheat_Age_0.png", "Wheat_Age_0");
    success &= loadTexture(wheatTextures[1], "../../textures/wheat/Wheat_Age_1.png", "Wheat_Age_1");
    success &= loadTexture(wheatTextures[2], "../../textures/wheat/Wheat_Age_2.png", "Wheat_Age_2");
    success &= loadTexture(wheatTextures[3], "../../textures/wheat/Wheat_Age_3.png", "Wheat_Age_3");
    success &= loadTexture(wheatTextures[4], "../../textures/wheat/Wheat_Age_4.png", "Wheat_Age_4");
    success &= loadTexture(wheatTextures[5], "../../textures/wheat/Wheat_Age_5.png", "Wheat_Age_5");
    success &= loadTexture(wheatTextures[6], "../../textures/wheat/Wheat_Age_6.png", "Wheat_Age_6");
    success &= loadTexture(wheatTextures[7], "../../textures/wheat/Wheat_Age_7.png", "Wheat_Age_7");

    success &= loadTexture(emeraldTexture, "../../textures/hud/Emerald.png", "Emerald");
    success &= loadTexture(chestTexture, "../../textures/hud/Chest.png", "Chest");
    success &= loadTexture(inventoryTexture, "../../textures/hud/Inventory.png", "Inventory");
    success &= loadTexture(hotbarTexture, "../../textures/hud/Hotbar.png", "Hotbar");
    success &= loadTexture(wheatItemTexture, "../../textures/items/Wheat_Item.png", "Wheat_Item");
    success &= loadTexture(wheatSeedsItemTexture, "../../textures/items/Wheat_Seeds_Item.png", "Wheat_Seeds_Item");

    if (!font.openFromFile("../../textures/font/Minecraft.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
        success = false;
    }

    blockTextureList.clear();
    blockTextureList.push_back(grassBlockTexture);
    blockTextureList.push_back(dirtTexture);
    blockTextureList.push_back(farmlandDryTexture);
    blockTextureList.push_back(farmlandWetTexture);
    blockTextureList.push_back(waterTexture);

    return success;
}

const sf::Texture& GameAssets::wheat(int age) const {
    if (age < 0) {
        return wheatTextures[0];
    }
    if (age > 7) {
        return wheatTextures[7];
    }
    return wheatTextures[age];
}
