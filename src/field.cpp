#include "../include/field.hpp"

Field::Field()
    : blockType(BlockType::NONE),
      cropState(CropState::EMPTY) {
    position = sf::Vector2f(0.0f, 0.0f);
}

void Field::setPosition(float x, float y) {
    position = sf::Vector2f(x, y);

    if (blockSprite.has_value()) {
        blockSprite->setPosition(position);
    }
}

void Field::setTopCenter(float x, float y) {
    topCenter = sf::Vector2f(x, y - ((TILE_SIZE + 16) * 0.78f));
}

void Field::setBlockType(BlockType type) {
    blockType = type;
}

void Field::setCropState(CropState state) {
    cropState = state;
}

void Field::setTexture(const sf::Texture& texture) {
    blockSprite = sf::Sprite(texture);

    blockSprite->setPosition(position);

    float effectiveWidth = texture.getSize().x - 32;
    float effectiveHeight = texture.getSize().y - 32;
    float scaleX = static_cast<float>(TILE_SIZE) / effectiveWidth;
    float scaleY = static_cast<float>(TILE_SIZE) / effectiveHeight;
    blockSprite->setScale(sf::Vector2f(scaleX, scaleY));

    blockSprite->setOrigin(sf::Vector2f(texture.getSize().x / 2.0f, texture.getSize().y));
}

void Field::setFocusTexture(const sf::Texture& texture, sf::RenderWindow& window) {
    focusSprite = sf::Sprite(texture);

    focusSprite->setPosition(position);

    float effectiveWidth = texture.getSize().x - 32;
    float effectiveHeight = texture.getSize().y - 32;
    float scaleX = static_cast<float>(TILE_SIZE) / effectiveWidth;
    float scaleY = static_cast<float>(TILE_SIZE) / effectiveHeight;
    focusSprite->setScale(sf::Vector2f(scaleX, scaleY));

    focusSprite->setOrigin(sf::Vector2f(texture.getSize().x / 2.0f, texture.getSize().y));
}

void Field::render(sf::RenderWindow& window) {
    if (blockType != BlockType::NONE && blockSprite.has_value()) {
        window.draw(*blockSprite);
    }

    if (isHovered == true && blockType != BlockType::NONE) {
        window.draw(*focusSprite);
    }
}

sf::Vector2f Field::getPosition() const {
    return position;
}

sf::Vector2f Field::getTopCenter() const {
    return topCenter;
}

BlockType Field::getBlockType() const {
    return blockType;
}

CropState Field::getCropState() const {
    return cropState;
}