#include "save_manager.hpp"
#include <algorithm>
#include <iostream>

bool SaveManager::ensureParentDirectoryExists(const std::filesystem::path& filePath) {
    const std::filesystem::path parentPath = filePath.parent_path();
    if (parentPath.empty()) {
        return true;
    }

    std::error_code errorCode;
    if (std::filesystem::create_directories(parentPath, errorCode)) {
        return true;
    }

    if (!errorCode && std::filesystem::exists(parentPath)) {
        return true;
    }

    std::cerr << "Failed to create save directory: " << parentPath.string()
              << " (" << errorCode.message() << ")" << std::endl;
    return false;
}

std::vector<SaveSlotInfo> SaveManager::listSaveFiles(const std::string& directory) {
    std::vector<SaveSlotInfo> saveFiles;

    std::error_code errorCode;
    const std::filesystem::path directoryPath(directory);
    if (!std::filesystem::exists(directoryPath, errorCode) || !std::filesystem::is_directory(directoryPath, errorCode)) {
        if (errorCode) {
            std::cerr << "Failed to access save directory: " << directory
                      << " (" << errorCode.message() << ")" << std::endl;
        }
        return saveFiles;
    }

    try {
        for (const auto& entry : std::filesystem::directory_iterator(directoryPath)) {
            if (!entry.is_regular_file()) {
                continue;
            }

            const std::filesystem::path filePath = entry.path();
            if (filePath.extension() != ".json") {
                continue;
            }

            SaveSlotInfo slotInfo;
            slotInfo.path = filePath.string();
            slotInfo.filename = filePath.filename().string();
            slotInfo.lastModified = entry.last_write_time();
            saveFiles.push_back(slotInfo);
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Failed to list save files in: " << directory << " (" << e.what() << ")" << std::endl;
        return {};
    }

    std::sort(saveFiles.begin(), saveFiles.end(), [](const SaveSlotInfo& left, const SaveSlotInfo& right) {
        return left.lastModified > right.lastModified;
    });

    return saveFiles;
}

std::string SaveManager::getLatestSavePath(const std::string& directory) {
    const std::vector<SaveSlotInfo> saveFiles = listSaveFiles(directory);
    if (saveFiles.empty()) {
        return "";
    }

    return saveFiles.front().path;
}

bool SaveManager::saveGame(const std::vector<std::vector<Field>>& fields, const std::string& filename, int emeraldCount, std::array<InventoryItem, 27>& inventoryItems) {
    try {
        if (!ensureParentDirectoryExists(filename)) {
            return false;
        }

        json saveData;

        saveData["version"] = "1.0";
        saveData["emeraldCount"] = emeraldCount;
        saveData["gridWidth"] = GRID_WIDTH;
        saveData["gridHeight"] = GRID_HEIGHT;

        json inventoryArray = json::array();
        for (int i = 0; i < 27; i++) {
            if (inventoryItems[i].id != 0) {
                json itemData;
                itemData["slot"] = i;
                itemData["id"] = inventoryItems[i].id;
                itemData["amount"] = inventoryItems[i].amount;
                inventoryArray.push_back(itemData);
            }
        }
        saveData["inventory"] = inventoryArray;

        json fieldsArray = json::array();

        for (int y = 0; y < GRID_HEIGHT; y++) {
            for (int x = 0; x < GRID_WIDTH; x++) {
                json fieldData;

                fieldData["x"] = x;
                fieldData["y"] = y;
                fieldData["blockType"] = blockTypeToString(fields[y][x].getBlockType());
                fieldData["cropState"] = cropStateToString(fields[y][x].getCropState());
                fieldData["cropType"] = cropTypeToString(fields[y][x].getCropType());
                fieldData["cropAge"] = fields[y][x].cropAge;

                if (fields[y][x].getBlockType() != BlockType::NONE) {
                    fieldsArray.push_back(fieldData);
                }
            }
        }

        saveData["fields"] = fieldsArray;

        std::ofstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file for writing: " << filename << std::endl;
            return false;
        }

        file << saveData.dump(4);
        file.close();

        std::cout << "Game saved successfully to: " << filename << std::endl;
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Error saving game: " << e.what() << std::endl;
        return false;
    }
}

bool SaveManager::loadGame(std::vector<std::vector<Field>>& fields, const std::string& filename, int& emeraldCount, std::array<InventoryItem, 27>& inventoryItems) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) {
            std::cerr << "Failed to open file for reading: " << filename << std::endl;
            return false;
        }

        json saveData;
        file >> saveData;
        file.close();

        if (saveData.contains("version")) {
            std::string version = saveData["version"];
            std::cout << "Loading save file version: " << version << std::endl;
        }

        if (saveData.contains("emeraldCount")) {
            emeraldCount = saveData["emeraldCount"];
        }

        if (saveData.contains("inventory")) {
            for (int i = 0; i < 27; i++) {
                inventoryItems[i].id = 0;
                inventoryItems[i].amount = 0;
            }

            for (const auto& itemData : saveData["inventory"]) {
                int slot = itemData["slot"];
                if (slot >= 0 && slot < 27) {
                    inventoryItems[slot].id = itemData["id"];
                    inventoryItems[slot].amount = itemData["amount"];
                }
            }
        }


        if (saveData.contains("gridWidth") && saveData.contains("gridHeight")) {
            int savedWidth = saveData["gridWidth"];
            int savedHeight = saveData["gridHeight"];

            if (savedWidth != GRID_WIDTH || savedHeight != GRID_HEIGHT) {
                std::cerr << "Warning: Save file grid dimensions (" << savedWidth << "x" << savedHeight << ") don't match current grid (" << GRID_WIDTH << "x" << GRID_HEIGHT << ")" << std::endl;
            }
        }

        for (int y = 0; y < GRID_HEIGHT; y++) {
            for (int x = 0; x < GRID_WIDTH; x++) {
                fields[y][x].setBlockType(BlockType::NONE);
                fields[y][x].setCropState(CropState::EMPTY);
                fields[y][x].setCropType(CropType::NONE);
                fields[y][x].cropAge = 0;
            }
        }

        if (saveData.contains("fields")) {
            for (const auto& fieldData : saveData["fields"]) {
                int x = fieldData["x"];
                int y = fieldData["y"];

                if (x >= 0 && x < GRID_WIDTH && y >= 0 && y < GRID_HEIGHT) {
                    BlockType blockType = stringToBlockType(fieldData["blockType"]);
                    CropState cropState = stringToCropState(fieldData["cropState"]);
                    CropType cropType = stringToCropType(fieldData["cropType"]);
                    int cropAge = fieldData["cropAge"];

                    fields[y][x].setBlockType(blockType);
                    fields[y][x].setCropState(cropState);
                    fields[y][x].setCropType(cropType);
                    fields[y][x].cropAge = cropAge;
                }
            }
        }

        std::cout << "Game loaded successfully from: " << filename << std::endl;
        return true;

    } catch (const std::exception& e) {
        std::cerr << "Error loading game: " << e.what() << std::endl;
        return false;
    }
}

std::string SaveManager::blockTypeToString(BlockType type) {
    switch (type) {
        case BlockType::NONE: return "none";
        case BlockType::GRASS_BLOCK: return "grass_block";
        case BlockType::DIRT: return "dirt";
        case BlockType::FARMLAND_DRY: return "farmland_dry";
        case BlockType::FARMLAND_WET: return "farmland_wet";
        case BlockType::WATER: return "water";
        default: return "none";
    }
}

std::string SaveManager::cropStateToString(CropState state) {
    switch (state) {
        case CropState::EMPTY: return "empty";
        case CropState::SEED: return "seed";
        case CropState::GROWN: return "grown";
        default: return "empty";
    }
}

std::string SaveManager::cropTypeToString(CropType type) {
    switch (type) {
        case CropType::NONE: return "none";
        case CropType::WHEAT: return "wheat";
        default: return "none";
    }
}

BlockType SaveManager::stringToBlockType(const std::string& str) {
    if (str == "grass_block") return BlockType::GRASS_BLOCK;
    if (str == "dirt") return BlockType::DIRT;
    if (str == "farmland_dry") return BlockType::FARMLAND_DRY;
    if (str == "farmland_wet") return BlockType::FARMLAND_WET;
    if (str == "water") return BlockType::WATER;
    return BlockType::NONE;
}

CropState SaveManager::stringToCropState(const std::string& str) {
    if (str == "seed") return CropState::SEED;
    if (str == "grown") return CropState::GROWN;
    return CropState::EMPTY;
}

CropType SaveManager::stringToCropType(const std::string& str) {
    if (str == "wheat") return CropType::WHEAT;
    return CropType::NONE;
}