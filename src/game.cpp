#include "../include/game.hpp"

#include <cmath>
#include <iostream>
#include <optional>

Game::Game()
    : window(sf::RenderWindow(sf::VideoMode({WINDOW_WIDTH, WINDOW_HEIGHT}), "Minecraft Farm Game")),
      saveFilePath(SAVES_DIR + "/" + DEFAULT_SAVE_FILE), autosave(saveFilePath) {
    window.setFramerateLimit(60);

    gameView.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    gameView.setCenter(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f));
    window.setView(gameView);
    zoomLevel = 1.0f;
    zoomStep = 1.1f;

    hudView.setSize(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    hudView.setCenter(sf::Vector2f(WINDOW_WIDTH / 2.0f, WINDOW_HEIGHT / 2.0f));

    assets.loadAll();
    world.initialize();

    if (!loadGame()) {
        std::cout << "No save file found, creating new game..." << std::endl;
        createNewGame();
    }
}

bool Game::loadGame() {
    bool success = SaveManager::loadGame(world.getFields(), saveFilePath, emeraldCount, inventory.items());
    if (success) {
        world.applyTextures(assets);
        std::cout << "GAME LOADED" << std::endl;
    }
    return success;
}

void Game::createNewGame() {
    world.createNewGame(assets.blockTextures());
    inventory.reset();
    inventory.addItem(1, 1);
    inventory.addItem(3, 10);
    inventory.addItem(4, 2);
    emeraldCount = 0;
}

void Game::run() {
    while (window.isOpen()) {
        clock();
        processEvents();
        render();
    }
}

void Game::handleWindowResize(unsigned int width, unsigned int height) {
    gameView.setSize(sf::Vector2f(width, height));
    handleZoom(zoomLevel);
    window.setView(gameView);

    hudView.setSize(sf::Vector2f(width, height));
    hudView.setCenter(sf::Vector2f(width / 2.0f, height / 2.0f));
}

void Game::handleZoom(float zoomLevel) {
    gameView.zoom(zoomLevel);
    window.setView(gameView);
}

void Game::handleViewMovement(sf::Vector2f worldPos) {
    gameView.move(worldLeftClickPos - worldPos);
    window.setView(gameView);
}

void Game::processEvents() {
    while (const std::optional event = window.pollEvent())
    {
        if (event->is<sf::Event::Closed>())
        {
            window.close();
        }
        else if (event->is<sf::Event::Resized>()) {
            handleWindowResize(event->getIf<sf::Event::Resized>()->size.x, event->getIf<sf::Event::Resized>()->size.y);
        }
        else if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
            if (keyPressed->code == sf::Keyboard::Key::S && keyPressed->control) {
                autosave.requestImmediateSave();
            }
            else if (keyPressed->code == sf::Keyboard::Key::L && keyPressed->control) {
                loadGame();
            }
            else if (keyPressed->code == sf::Keyboard::Key::N && keyPressed->control) {
                createNewGame();
            }
        }
        else if (const auto* mouseWheelScrolled = event->getIf<sf::Event::MouseWheelScrolled>()) {
            if(!inventory.isOpen()) {
                if (mouseWheelScrolled->delta <= 0) {
                    if (zoomLevel < 1.5f) {
                        zoomLevel *= powf(zoomStep, 1);
                        handleZoom(powf(zoomStep, 1));
                    }
                }
                else {
                    if (zoomLevel > 0.5f) {
                        zoomLevel *= powf(zoomStep, -1);
                        handleZoom(powf(zoomStep, -1));
                    }
                }
            }
        }
        else if (const auto* mouseMoved = event->getIf<sf::Event::MouseMoved>()) {
            sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
            sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
            world.updateMouseFocus(worldPos.x, worldPos.y, mouseButtonLeftPressed, assets.focused(), window);

            if (mouseButtonLeftPressed == true && !inventory.isOpen()) {
                handleViewMovement(worldPos);
            }
        }
        else if (const auto* mouseButtonPressed = event->getIf<sf::Event::MouseButtonPressed>()) {
            if (mouseButtonPressed->button == sf::Mouse::Button::Left) {
                sf::Vector2i pixelLeftClickPos = mouseButtonPressed->position;
                worldLeftClickPos = window.mapPixelToCoords(pixelLeftClickPos);

                mouseButtonLeftPressed = true;

                sf::Vector2f hudMousePos = window.mapPixelToCoords(pixelLeftClickPos, hudView);
                inventory.handleLeftClick(hudMousePos);
            }
            else if (mouseButtonPressed->button == sf::Mouse::Button::Right) {
                sf::Vector2i pixelRightClickPos = mouseButtonPressed->position;
                worldRightClickPos = window.mapPixelToCoords(pixelRightClickPos);

                if (inventory.isOpen()) {
                    return;
                }

                if(!inventory.isOpen()) {
                    if (!world.hasFocusedField()) {
                        std::cout << "mouse right: " << worldRightClickPos.x << ", " << worldRightClickPos.y << " | field: outside the map" << std::endl;
                        return;
                    }

                    if (world.plant(inventory.selectedItem(), assets)) {
                        inventory.clearSelectionIfEmpty();
                        autosave.updateSnapshotAndMarkDirty(world.getFields(), inventory.items(), emeraldCount);
                    }

                    if (emeraldCount >= 3) {
                        if (world.till(inventory.selectedItem(), assets)) {
                            emeraldCount -= 3;
                            autosave.updateSnapshotAndMarkDirty(world.getFields(), inventory.items(), emeraldCount);
                        }
                    }

                    const HarvestRewards rewards = world.harvest();

                    if (rewards.emeralds > 0 || !rewards.droppedItems.empty()) {
                        emeraldCount += rewards.emeralds;

                        for (const auto& item : rewards.droppedItems) {
                            int itemID = item.first;
                            int amount = item.second;
                            inventory.addItem(itemID, amount);
                        }

                        autosave.updateSnapshotAndMarkDirty(world.getFields(), inventory.items(), emeraldCount);
                    }
                }

                const sf::Vector2f focused = world.focusedField();
                std::cout << "mouse right: " << worldRightClickPos.x << ", " << worldRightClickPos.y << " | field: " << focused.x << ", " << focused.y << std::endl;
            }
        }
        else if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
            if (mouseButtonReleased->button == sf::Mouse::Button::Left) {
                mouseButtonLeftPressed = false;
            }
        }
    }
}

void Game::clock() {
    sf::Time gameTime = gameClock.getElapsedTime();

    if (gameTime.asMilliseconds() >= 1000) {
        gameClock.restart();
        world.tick(assets);
    }
}

void Game::render() {
    window.clear(sf::Color(110, 177, 255));

    world.renderFields(window);

    if (!inventory.isOpen()) {
        world.renderFocus(window);
    }

    window.setView(hudView);
    inventory.renderHud(window, hudView, assets, emeraldCount);
    if (inventory.isOpen()) {
        inventory.renderInventory(window, hudView, assets);
    }

    window.setView(gameView);

    window.display();
}