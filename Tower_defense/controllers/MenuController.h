#pragma once

#include <SFML/Graphics.hpp>
#include "../models/GameModel.h"
#include "../views/MenuView.h"

class MenuController {
public:
    MenuController(GameModel& model, MenuView& view);
    void handleInput(sf::Event& event, sf::RenderWindow& window);
    void handleMouseClick(const sf::Vector2f& mousePos);
    void toggleSound();

private:
    GameModel& model;
    MenuView& view;
    bool soundEnabled;
}; 