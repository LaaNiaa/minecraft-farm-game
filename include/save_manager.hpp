#pragma once

#include <filesystem>
#include <string>
#include <vector>
#include <fstream>
#include <nlohmann/json.hpp>
#include "const.hpp"
#include "field.hpp"

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

class SaveManager {
    public:
        static bool saveGame(const std::vector<std::vector<Field>>& fields, const std::string& filename, int emeraldCount, std::array<InventoryItem, 27>& inventoryItems);

        static bool loadGame(std::vector<std::vector<Field>>& fields, const std::string& filename, int& emeraldCount, std::array<InventoryItem, 27>& inventoryItems);

        static std::vector<SaveSlotInfo> listSaveFiles(const std::string& directory = SAVES_DIR);
        static std::string getLatestSavePath(const std::string& directory = SAVES_DIR);

    private:
        static bool ensureParentDirectoryExists(const std::filesystem::path& filePath);

        static std::string blockTypeToString(BlockType type);
        static std::string cropStateToString(CropState state);
        static std::string cropTypeToString(CropType type);

        static BlockType stringToBlockType(const std::string& str);
        static CropState stringToCropState(const std::string& str);
        static CropType stringToCropType(const std::string& str);
};