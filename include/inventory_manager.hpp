#pragma once

#include <SFML/Graphics.hpp>
#include <array>

#include "save_manager.hpp"

class GameAssets;

class InventoryManager {
public:
    void reset();

    void handleLeftClick(const sf::Vector2f& hudMousePos);
    bool isOpen() const { return inventoryOpened; }

    InventoryItem* selectedItem();
    void clearSelectionIfEmpty();

    void renderHud(sf::RenderWindow& window, const sf::View& hudView, const GameAssets& assets, int emeraldCount);
    void renderInventory(sf::RenderWindow& window, const sf::View& hudView, const GameAssets& assets);

    void addItem(int itemID, int amount);

    std::array<InventoryItem, 27>& items() { return inventoryItems; }
    const std::array<InventoryItem, 27>& items() const { return inventoryItems; }

private:
    const sf::Texture& textureForItem(const GameAssets& assets, int itemId) const;

    sf::FloatRect chestBound;
    sf::FloatRect inventoryBound;
    sf::FloatRect hotbarBound;
    bool inventoryOpened = false;

    static constexpr float InventorySlotSize = 16.0f;
    static constexpr float InventorySpacing = 2.0f;
    static constexpr float InventoryOffset = 11.0f;
    static constexpr int ItemStackLimit = 64;

    int selectedItemIndex = -1;
    std::array<sf::FloatRect, 27> slots;
    std::array<InventoryItem, 27> inventoryItems = {};
};
