#pragma once

#include <SFML/Graphics.hpp>
#include <array>
#include <string>
#include <vector>

class GameAssets {
public:
    bool loadAll();

    const sf::Texture& focused() const { return focusedTexture; }
    const sf::Texture& grass() const { return grassBlockTexture; }
    const sf::Texture& dirt() const { return dirtTexture; }
    const sf::Texture& farmlandDry() const { return farmlandDryTexture; }
    const sf::Texture& farmlandWet() const { return farmlandWetTexture; }
    const sf::Texture& water() const { return waterTexture; }

    const sf::Texture& wheat(int age) const;

    const sf::Texture& emerald() const { return emeraldTexture; }
    const sf::Texture& chest() const { return chestTexture; }
    const sf::Texture& inventory() const { return inventoryTexture; }
    const sf::Texture& hotbar() const { return hotbarTexture; }

    const sf::Texture& wheatItem() const { return wheatItemTexture; }
    const sf::Texture& wheatSeedsItem() const { return wheatSeedsItemTexture; }

    const sf::Font& gameFont() const { return font; }
    const std::vector<sf::Texture>& blockTextures() const { return blockTextureList; }

private:
    bool loadTexture(sf::Texture& texture, const std::string& path, const std::string& label) const;

    sf::Texture focusedTexture;
    sf::Texture grassBlockTexture;
    sf::Texture dirtTexture;
    sf::Texture farmlandDryTexture;
    sf::Texture farmlandWetTexture;
    sf::Texture waterTexture;

    std::array<sf::Texture, 8> wheatTextures;

    sf::Texture emeraldTexture;
    sf::Texture chestTexture;
    sf::Texture inventoryTexture;
    sf::Texture hotbarTexture;

    sf::Texture wheatItemTexture;
    sf::Texture wheatSeedsItemTexture;

    sf::Font font;
    std::vector<sf::Texture> blockTextureList;
};
