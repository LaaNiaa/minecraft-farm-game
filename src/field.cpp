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

void Field::setBlockType(BlockType type) {
    blockType = type;
}

void Field::setCropState(CropState state) {
    cropState = state;
}

void Field::setTexture(const sf::Texture& texture) {
    blockSprite = sf::Sprite(texture);

    blockSprite->setPosition(position);

    float scaleX = static_cast<float>(TILE_SIZE) / texture.getSize().x;
    float scaleY = static_cast<float>(TILE_SIZE) / texture.getSize().y;
    blockSprite->setScale(sf::Vector2f(scaleX, scaleY));

    blockSprite->setOrigin(sf::Vector2f(texture.getSize().x / 2.0f, texture.getSize().y));
}

void Field::render(sf::RenderWindow& window) {
    if (blockType != BlockType::NONE && blockSprite.has_value()) {
        window.draw(*blockSprite);
    }
}

sf::Vector2f Field::getPosition() const {
    return position;
}

BlockType Field::getBlockType() const {
    return blockType;
}

CropState Field::getCropState() const {
    return cropState;
}