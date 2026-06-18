#include "../include/game_assets.hpp"

#include <iostream>
#include <filesystem>
#include <regex>
#include <string>
#include <algorithm>
#include <cctype>

bool GameAssets::loadTexture(sf::Texture& texture, const std::string& path, const std::string& label) const {
    if (!texture.loadFromFile(path)) {
        std::cerr << "Failed to load " << label << " texture" << std::endl;
        return false;
    }
    return true;
}

bool GameAssets::loadAll() {
    bool success = true;

    std::regex cropPattern(R"(([A-Za-z]+)_Age_([0-9]+)\.png)");
    std::smatch match;

    const std::string cropsRootDirectory = "../../textures/crops";

    int loadedWheatPhases = 0;
    // int loadedPotatoPhases = 0;

    try {
        if (std::filesystem::exists(cropsRootDirectory)) {
            for (const auto& entry : std::filesystem::recursive_directory_iterator(cropsRootDirectory)) {

                if (!entry.is_regular_file()) continue;

                std::string filename = entry.path().filename().string();

                if (std::regex_match(filename, match, cropPattern)) {
                    std::string cropName = match[1].str();
                    std::transform(cropName.begin(), cropName.end(), cropName.begin(),
                        [](unsigned char c){ return std::tolower(c); });
                    int age = std::stoi(match[2].str());

                    if (cropName == "wheat") {
                        if (age >= 0 && age < static_cast<int>(wheatTextures.size())) {
                            if (loadTexture(wheatTextures[age], entry.path().string(), "Wheat_Age_" + std::to_string(age))) {
                                loadedWheatPhases++;
                            } else {
                                success = false;
                            }
                        }
                    }
                    /*
                     *
                     */
                }
            }
        }

        if (loadedWheatPhases != 8) {
            std::cerr << "[GameAssets] BLAD: Nie zaladowano 8 faz pszenicy (znaleziono " << loadedWheatPhases << ")" << std::endl;
            success = false;
        }

    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "[GameAssets] Blad skanowania rekursywnego: " << e.what() << std::endl;
        success = false;
    }

    success &= loadTexture(grassBlockTexture, "../../textures/Grass_Block.png", "Grass_Block");
    success &= loadTexture(focusedTexture, "../../textures/Focused.png", "Focused");
    success &= loadTexture(dirtTexture, "../../textures/Dirt.png", "Dirt");
    success &= loadTexture(farmlandDryTexture, "../../textures/Farmland_Dry.png", "Farmland_Dry");
    success &= loadTexture(farmlandWetTexture, "../../textures/Farmland_Wet.png", "Farmland_Wet");
    success &= loadTexture(waterTexture, "../../textures/Water.png", "Water");

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
