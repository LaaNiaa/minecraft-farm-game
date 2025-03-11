#pragma once

#include <SFML/Graphics.hpp>
#include <optional>
#include "const.hpp"

class Field {
    public:
        Field();
        void setPosition(float x, float y);
        void setBlockType(BlockType type);
        void setCropState(CropState state);
        void setTexture(const sf::Texture& texture);

        void render(sf::RenderWindow& window);

        sf::Vector2f getPosition() const;
        BlockType getBlockType() const;
        CropState getCropState() const;

    private:
        std::optional<sf::Sprite> blockSprite;

        sf::Vector2f position;
        BlockType blockType;
        CropState cropState;
};