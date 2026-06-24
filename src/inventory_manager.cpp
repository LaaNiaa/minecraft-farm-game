#include "../include/inventory_manager.hpp"

#include "../include/game_assets.hpp"

#include <algorithm>
#include <iostream>

void InventoryManager::reset() {
    for (InventoryItem& item : inventoryItems) {
        item.id = 0;
        item.amount = 0;
    }
    selectedItemIndex = -1;
    inventoryOpened = false;
}

void InventoryManager::handleLeftClick(const sf::Vector2f& hudMousePos) {
    if (inventoryOpened) {
        for (int i = 0; i < static_cast<int>(slots.size()); i++) {
            if (slots[i].contains(hudMousePos) && inventoryItems[i].id != 0) {
                selectedItemIndex = i;
                inventoryOpened = false;
                return;
            }
        }
    }

    if (!inventoryOpened && chestBound.contains(hudMousePos)) {
        inventoryOpened = true;
        return;
    }

    if (inventoryOpened && inventoryBound.contains(hudMousePos)) {
        return;
    }

    if (inventoryOpened && !inventoryBound.contains(hudMousePos)) {
        inventoryOpened = false;
    }
}

InventoryItem* InventoryManager::selectedItem() {
    if (selectedItemIndex < 0 || selectedItemIndex >= static_cast<int>(inventoryItems.size())) {
        return nullptr;
    }
    if (inventoryItems[selectedItemIndex].id == 0 || inventoryItems[selectedItemIndex].amount <= 0) {
        return nullptr;
    }
    return &inventoryItems[selectedItemIndex];
}

void InventoryManager::clearSelectionIfEmpty() {
    if (selectedItemIndex < 0 || selectedItemIndex >= static_cast<int>(inventoryItems.size())) {
        selectedItemIndex = -1;
        return;
    }

    InventoryItem& item = inventoryItems[selectedItemIndex];
    if (item.id == 0 || item.amount <= 0) {
        item.id = 0;
        item.amount = 0;
        selectedItemIndex = -1;
    }
}

const sf::Texture& InventoryManager::textureForItem(const GameAssets& assets, int itemId) const {
    if (itemId == 1) {
        return assets.ironHoe();
    }
    if (itemId == 2) {
        return assets.wheatItem();
    }
    if (itemId == 3) {
        return assets.wheatSeedsItem();
    }
    if (itemId == 4) {
        return assets.carrotItem();
    }

    else {
        return assets.emerald();
    }
}

void InventoryManager::renderHud(sf::RenderWindow& window, const sf::View& hudView, const GameAssets& assets, int emeraldCount) {
    const float margin = 30.0f;
    const float iconSize = 48.0f;

    sf::Sprite emeraldSprite(assets.emerald());
    const sf::Vector2u texSize = assets.emerald().getSize();
    const float scaleX = iconSize / static_cast<float>(texSize.x);
    const float scaleY = iconSize / static_cast<float>(texSize.y);
    emeraldSprite.setScale(sf::Vector2f(scaleX, scaleY));
    emeraldSprite.setPosition(sf::Vector2f(margin - (30.0f * scaleX), margin - (20.0f * scaleY)));
    window.draw(emeraldSprite);

    sf::Text countText(assets.gameFont(), std::to_string(emeraldCount), 32);
    countText.setFillColor(sf::Color::White);
    countText.setOutlineColor(sf::Color::Black);
    countText.setOutlineThickness(2.0f);
    countText.setPosition(sf::Vector2f(margin + iconSize + 10.0f, margin + (iconSize * 0.1f) - (20.0f * scaleY)));
    window.draw(countText);

    const float chestIconSize = 150.0f;
    sf::Sprite chestSprite(assets.chest());
    const sf::Vector2u chestTexSize = assets.chest().getSize();
    const float chestScaleX = chestIconSize / static_cast<float>(chestTexSize.x);
    const float chestScaleY = chestIconSize / static_cast<float>(chestTexSize.y);
    chestSprite.setScale(sf::Vector2f(chestScaleX, chestScaleY));
    chestSprite.setPosition(sf::Vector2f(margin - (42.0f * chestScaleX), hudView.getSize().y - chestIconSize - margin - (3.0f * chestScaleY)));
    chestBound = chestSprite.getGlobalBounds();
    window.draw(chestSprite);

    const float hotbarSize = 100.0f;
    sf::Sprite hotbarSprite(assets.hotbar());
    const sf::Vector2u hotbarTexSize = assets.hotbar().getSize();
    const float hotbarScaleX = hotbarSize / static_cast<float>(hotbarTexSize.x);
    const float hotbarScaleY = hotbarSize / static_cast<float>(hotbarTexSize.y);
    hotbarSprite.setScale(sf::Vector2f(hotbarScaleX, hotbarScaleY));
    hotbarSprite.setPosition(sf::Vector2f(hudView.getSize().x - hotbarSize - margin, hudView.getSize().y - hotbarSize - margin));
    hotbarBound = hotbarSprite.getGlobalBounds();
    window.draw(hotbarSprite);

    InventoryItem* selected = selectedItem();
    if (selected == nullptr) {
        return;
    }

    sf::Sprite selectedItemSprite(textureForItem(assets, selected->id));
    selectedItemSprite.setPosition(sf::Vector2f(hudView.getSize().x - hotbarSize - margin + (4.0f * hotbarScaleX),
                                                hudView.getSize().y - hotbarSize - margin + (4.0f * hotbarScaleX)));
    const float selectedItemScale = (hotbarSize - (8.0f * hotbarScaleX)) / 160.0f;
    selectedItemSprite.setScale(sf::Vector2f(selectedItemScale, selectedItemScale));
    window.draw(selectedItemSprite);

    sf::Text selectedItemCountText(assets.gameFont(), std::to_string(selected->amount), 32);
    selectedItemCountText.setFillColor(sf::Color::White);
    selectedItemCountText.setOutlineColor(sf::Color::Black);
    selectedItemCountText.setOutlineThickness(2.0f);
    if (selected->amount > 9) {
        selectedItemCountText.setPosition(sf::Vector2f(hudView.getSize().x - hotbarSize - margin + (4.0f * hotbarScaleX) + 32.0f,
                                                       hudView.getSize().y - hotbarSize - margin + (4.0f * hotbarScaleX) + 32.0f));
    } else if (selected->amount > 0) {
        selectedItemCountText.setPosition(sf::Vector2f(hudView.getSize().x - hotbarSize - margin + (4.0f * hotbarScaleX) + 48.0f,
                                                       hudView.getSize().y - hotbarSize - margin + (4.0f * hotbarScaleX) + 32.0f));
    }
    window.draw(selectedItemCountText);
}

void InventoryManager::renderInventory(sf::RenderWindow& window, const sf::View& hudView, const GameAssets& assets) {
    const float inventorySize = hudView.getSize().x * 0.65f;
    sf::Sprite inventorySprite(assets.inventory());

    const sf::Vector2u inventoryTexSize = assets.inventory().getSize();
    const float inventoryScale = inventorySize / static_cast<float>(inventoryTexSize.x);
    inventorySprite.setScale(sf::Vector2f(inventoryScale, inventoryScale));
    inventorySprite.setPosition(sf::Vector2f(hudView.getCenter().x - (inventoryTexSize.x * inventoryScale) / 2.0f - (3.0f * inventoryScale),
                                             hudView.getCenter().y - (inventoryTexSize.y * inventoryScale) / 2.0f - (3.0f * inventoryScale)));
    inventoryBound = inventorySprite.getGlobalBounds();
    const sf::Vector2f inventoryPos = inventorySprite.getPosition();
    window.draw(inventorySprite);

    int slotIndex = 0;
    for (int row = 0; row < 3; row++) {
        for (int col = 0; col < 9; col++) {
            const float slotX = inventoryPos.x + (InventoryOffset + col * (InventorySlotSize + InventorySpacing)) * inventoryScale;
            const float slotY = inventoryPos.y + (InventoryOffset + row * (InventorySlotSize + InventorySpacing)) * inventoryScale;

            slots[slotIndex] = sf::FloatRect(sf::Vector2f(slotX, slotY), sf::Vector2f(InventorySlotSize * inventoryScale, InventorySlotSize * inventoryScale));

            if (inventoryItems[slotIndex].id != 0) {
                sf::Sprite itemSprite(textureForItem(assets, inventoryItems[slotIndex].id));
                const float itemScale = (InventorySlotSize * inventoryScale) / 160.0f;
                itemSprite.setScale(sf::Vector2f(itemScale, itemScale));
                itemSprite.setPosition(sf::Vector2f(slotX, slotY));
                window.draw(itemSprite);
            }

            if (inventoryItems[slotIndex].amount > 0) {
                sf::Text itemCountText(assets.gameFont(), std::to_string(inventoryItems[slotIndex].amount), 32);
                itemCountText.setFillColor(sf::Color::White);
                itemCountText.setOutlineColor(sf::Color::Black);
                itemCountText.setOutlineThickness(2.0f);

                if (inventoryItems[slotIndex].amount > 9) {
                    itemCountText.setPosition(sf::Vector2f(slotX + (InventorySlotSize * inventoryScale) - 32.0f - ((InventorySpacing * inventoryScale) / 2.0f),
                                                           slotY + (InventorySlotSize * inventoryScale) - 32.0f - ((InventorySpacing * inventoryScale) / 2.0f)));
                } else {
                    itemCountText.setPosition(sf::Vector2f(slotX + (InventorySlotSize * inventoryScale) - 16.0f - ((InventorySpacing * inventoryScale) / 2.0f),
                                                           slotY + (InventorySlotSize * inventoryScale) - 32.0f - ((InventorySpacing * inventoryScale) / 2.0f)));
                }
                window.draw(itemCountText);
            }

            slotIndex++;
        }
    }
}

void InventoryManager::addItem(int itemID, int amount) {
    int remaining = amount;
    for (int i = 0; i < static_cast<int>(inventoryItems.size()) && remaining > 0; i++) {
        if (inventoryItems[i].id == itemID && inventoryItems[i].amount < ItemStackLimit) {
            const int spaceInSlot = ItemStackLimit - inventoryItems[i].amount;
            const int toAdd = std::min(remaining, spaceInSlot);
            inventoryItems[i].amount += toAdd;
            remaining -= toAdd;
        }
    }

    for (int i = 0; i < static_cast<int>(inventoryItems.size()) && remaining > 0; i++) {
        if (inventoryItems[i].id == 0) {
            const int toAdd = std::min(remaining, ItemStackLimit);
            inventoryItems[i].id = itemID;
            inventoryItems[i].amount = toAdd;
            remaining -= toAdd;
        }
    }

    if (remaining > 0) {
        std::cout << "Inventory full! Could not add " << remaining << " items." << std::endl;
    }
}
