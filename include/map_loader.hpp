#pragma once

#include <string>
#include <vector>
#include "const.hpp"
#include "field.hpp"

class MapLoader {
      public:
        static std::vector<std::vector<int>> loadMapFromFile(const std::string& fileName);
        static void applyMapToFields(std::vector<std::vector<Field>>& fields,
                                     const std::vector<std::vector<int>>& mapData,
                                     const std::vector<sf::Texture>& textures);

      private:
        static BlockType intToBlockType(int value);
};