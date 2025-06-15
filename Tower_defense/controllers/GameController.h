#pragma once

#include <SFML/Graphics.hpp>
#include "../models/GameModel.h"
#include "../views/GameView.h"

class GameController {
public:
    GameController(GameModel& model, GameView& view);
    void handleInput(sf::Event& event, sf::RenderWindow& window);
    void handleMouseClick(const sf::Vector2f& mousePos);
    void handleTowerPlacement(const sf::Vector2i& gridPos);
    void handleTowerUpgrade(size_t towerIndex);
    void handleTowerSell(size_t towerIndex);

private:
    GameModel& model;
    GameView& view;
    bool menuJustOpened;
    int selectedTowerIndex;
    sf::Vector2i selectedGridPos;
}; 