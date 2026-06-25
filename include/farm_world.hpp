#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

#include "const.hpp"
#include "field.hpp"
#include "save_manager.hpp"
#include <utility>

class GameAssets;

struct HarvestRewards {
    int emeralds = 0;
    std::vector<std::pair<int, int>> droppedItems;
};

class FarmWorld {
public:
    void initialize();
    void applyTextures(const GameAssets& assets);
    void createNewGame(const std::vector<sf::Texture>& blockTextures);

    void updateMouseFocus(float mousePosX, float mousePosY, bool mouseButtonLeftPressed, const sf::Texture& focusTexture, sf::RenderWindow& window);
    bool hasFocusedField() const;
    sf::Vector2f focusedField() const { return focusedFieldCoords; }

    bool plant(InventoryItem* selectedItem, const GameAssets& assets);
    bool till(InventoryItem* selectedItem, const GameAssets& assets);
    HarvestRewards harvest();

    void tick(const GameAssets& assets);
    void renderFields(sf::RenderWindow& window);
    void renderFocus(sf::RenderWindow& window);

    std::vector<std::vector<Field>>& getFields() { return fields; }

private:
    void updateFieldPositions();
    void farmlandHydration(const sf::Texture& farmlandWetTexture);
    void plantGrowth(const GameAssets& assets);

    std::vector<std::vector<Field>> fields;
    sf::Vector2f focusedFieldCoords = sf::Vector2f(-1.0f, -1.0f);
};
