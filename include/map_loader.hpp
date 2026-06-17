#pragma once

#include <string>
#include <vector>
#include "const.hpp"
#include "field.hpp"

struct MapFileInfo {
    std::string path;
    std::string filename;
};

class MapLoader {
      public:
        static std::vector<std::vector<int>> loadMapFromFile(const std::string& fileName);
        static void applyMapToFields(std::vector<std::vector<Field>>& fields,
                                     const std::vector<std::vector<int>>& mapData,
                                     const std::vector<sf::Texture>& textures);

        static std::vector<MapFileInfo> listMapFiles(const std::string& directory = MAPS_DIR);
        static std::string resolveDefaultMapPath(const std::string& directory = MAPS_DIR);

      private:
        static BlockType intToBlockType(int value);
};