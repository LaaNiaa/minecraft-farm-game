#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
#include "const.hpp"
#include "field.hpp"
#include "enum_utils.hpp"

using json = nlohmann::json;

struct InventoryItem {
    int id = 0;
    int amount = 0;
};

struct SaveSlotInfo {
    std::string path;
    std::string filename;
    std::filesystem::file_time_type lastModified;
};

struct FieldSnapshot {
    BlockType blockType;
    CropState cropState;
    CropType cropType;
    int cropAge;
};

struct GameSnapshot {
    int emeraldCount = 0;
    std::array<InventoryItem, 27> inventoryItems;
    std::vector<std::vector<FieldSnapshot>> fields;
};

class SaveManager {
    public:
        static bool loadGame(std::vector<std::vector<Field>>& fields, const std::string& filename, int& emeraldCount, std::array<InventoryItem, 27>& inventoryItems);

        static bool saveSnapshot(const GameSnapshot& snapshot, const std::string& filename);

        static std::vector<SaveSlotInfo> listSaveFiles(const std::string& directory = SAVES_DIR);
        static std::string getLatestSavePath(const std::string& directory = SAVES_DIR);

    private:
        static bool ensureParentDirectoryExists(const std::filesystem::path& filePath);
};