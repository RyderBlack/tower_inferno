#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include "tower.h"

bool isInside(const sf::Vector2i& point, const sf::RectangleShape& rect) {
    return rect.getGlobalBounds().contains(static_cast<sf::Vector2f>(point));
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Tower Defense avec Factory Pattern");

    sf::Texture grassTexture, sandTexture, goldTexture, heartTexture;
    if (!grassTexture.loadFromFile("assets/grass_tile.png") ||
        !sandTexture.loadFromFile("assets/sand_tile.png") ||
        !goldTexture.loadFromFile("assets/gold.png") ||
        !heartTexture.loadFromFile("assets/heart.png")) {
        std::cerr << "Erreur lors du chargement des textures!" << std::endl;
        return -1;
    }

    sf::Font font;
    if (!font.loadFromFile("assets/fast99.ttf")) {
        std::cerr << "Erreur lors du chargement de la police!" << std::endl;
        return -1;
    }

    sf::Text goldText;
    goldText.setFont(font);
    goldText.setCharacterSize(30);
    goldText.setFillColor(sf::Color::Yellow);
    goldText.setPosition(70.f, 25.f);
    sf::Sprite goldSprite(goldTexture);
    goldSprite.setPosition(20.f, 20.f);

    int lives = 3;
    int gold = 5000;
    const int rows = 20, cols = 20, tileSize = 40;
    int grid[rows][cols] = {0};

    std::vector<sf::Vector2i> path = {
        {0,5},{1,5},{2,5},{3,5},{4,5},{5,5},{5,6},{5,7},{5,8},{5,9},{5,10},
        {6,10},{7,10},{8,10},{9,10},{10,10},{11,10},{12,10},{13,10},{14,10},
        {14,9},{14,8},{14,7},{14,6},{14,5},{14,4},{14,3},{15,3},{16,3},{17,3},{18,3},{19,3}
    };
    for (auto& p : path) {
        grid[p.y][p.x] = 1;
    }

    std::vector<std::unique_ptr<Tower>> towers;
    TowerSelectionUI towerUI(&font);

    sf::RectangleShape upgradeButton({100.f, 30.f});
    sf::RectangleShape sellButton({100.f, 30.f});
    upgradeButton.setFillColor(sf::Color(100, 100, 255));
    sellButton.setFillColor(sf::Color(255, 100, 100));

    sf::Text upgradeText("Upgrade", font, 20);
    sf::Text sellText("Vendre", font, 20);
    upgradeText.setFillColor(sf::Color::White);
    sellText.setFillColor(sf::Color::White);

    bool showTowerButtons = false;
    bool menuJustOpened = false;
    int selectedTowerIndex = -1;
    sf::Vector2f buttonPosition;
    sf::Vector2i selectedGridPos;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left) {

                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                sf::Vector2i gridPos(mousePos.x / tileSize, mousePos.y / tileSize);

                if (towerUI.isVisible()) {
                    TowerType selectedType = towerUI.checkClick(mousePos);

                    if (selectedGridPos.x >= 0 && selectedGridPos.x < cols && 
                        selectedGridPos.y >= 0 && selectedGridPos.y < rows &&
                        grid[selectedGridPos.y][selectedGridPos.x] == 0 && 
                        gold >= TowerFactory::getTowerCost(selectedType)) {
                        
                        bool alreadyPlaced = false;
                        for (auto& tower : towers) {
                            if (tower->position == selectedGridPos) {
                                alreadyPlaced = true;
                                break;
                            }
                        }

                        if (!alreadyPlaced) {
                            auto newTower = TowerFactory::createTower(selectedType, selectedGridPos);
                            if (newTower) {
                                gold -= newTower->cost;
                                towers.push_back(std::move(newTower));
                            }
                        }
                    }
                    towerUI.hide();
                    continue;
                }

                if (showTowerButtons && !menuJustOpened) {
                    if (isInside(mousePos, upgradeButton)) {
                        if (selectedTowerIndex >= 0 &&
                            towers[selectedTowerIndex]->level < towers[selectedTowerIndex]->maxLevel &&
                            gold >= towers[selectedTowerIndex]->upgradeCost) {
                            towers[selectedTowerIndex]->upgrade();
                            gold -= towers[selectedTowerIndex]->upgradeCost;
                        }
                        showTowerButtons = false;
                        selectedTowerIndex = -1;
                        continue;
                    } else if (isInside(mousePos, sellButton)) {
                        if (selectedTowerIndex >= 0) {
                            gold += towers[selectedTowerIndex]->getSellPrice();
                            towers.erase(towers.begin() + selectedTowerIndex);
                        }
                        showTowerButtons = false;
                        selectedTowerIndex = -1;
                        continue;
                    }
                }

                bool clickedOnTower = false;
                for (size_t i = 0; i < towers.size(); ++i) {
                    if (towers[i]->position == gridPos) {
                        selectedTowerIndex = static_cast<int>(i);
                        buttonPosition = sf::Vector2f(mousePos);
                        upgradeButton.setPosition(buttonPosition);
                        sellButton.setPosition(buttonPosition.x, buttonPosition.y + 35);
                        upgradeText.setPosition(buttonPosition.x + 10, buttonPosition.y + 2);
                        sellText.setPosition(buttonPosition.x + 10, buttonPosition.y + 37);
                        showTowerButtons = true;
                        menuJustOpened = true;
                        clickedOnTower = true;
                        break;
                    }
                }

                if (!clickedOnTower && gridPos.x >= 0 && gridPos.x < cols && 
                    gridPos.y >= 0 && gridPos.y < rows && grid[gridPos.y][gridPos.x] == 0) {
                    selectedGridPos = gridPos; 
                    towerUI.show(sf::Vector2f(mousePos));
                    showTowerButtons = false;
                    selectedTowerIndex = -1;
                } else if (!clickedOnTower) {
                    showTowerButtons = false;
                    selectedTowerIndex = -1;
                }
            }
        }

        window.clear();

        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
                sf::Sprite tile(grid[y][x] == 1 ? sandTexture : grassTexture);
                tile.setPosition(x * tileSize, y * tileSize);
                window.draw(tile);
            }
        }

        for (auto& tower : towers) {
            tower->render(window, tileSize);
        }

        goldText.setString(std::to_string(gold));
        window.draw(goldSprite);
        window.draw(goldText);

        for (int i = 0; i < lives; ++i) {
            sf::Sprite heart(heartTexture);
            heart.setPosition(window.getSize().x - (i + 1) * (heart.getGlobalBounds().width + 10), 10.f);
            window.draw(heart);
        }

        if (showTowerButtons) {
            window.draw(upgradeButton);
            window.draw(sellButton);
            window.draw(upgradeText);
            window.draw(sellText);
        }

        towerUI.render(window);

        window.display();
        menuJustOpened = false;
    }

    return 0;
}