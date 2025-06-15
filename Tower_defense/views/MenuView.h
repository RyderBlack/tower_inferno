#pragma once

#include <SFML/Graphics.hpp>
#include "../models/GameModel.h"
#include "../models/ScoreModel.h"

class MenuView {
public:
    MenuView();
    void render(sf::RenderWindow& window, const GameModel& model, const ScoreModel& scoreModel);
    void loadResources();
    void setSoundState(bool enabled);

    // Make UI elements accessible to the controller
    sf::Sprite playButton;
    sf::Sprite scoreButton;
    sf::Sprite quitButton;
    sf::Sprite volumeButton;
    sf::Text scoreBackButton;

private:
    // Textures
    sf::Texture backgroundTexture;
    sf::Texture playTexture;
    sf::Texture scoreTexture;
    sf::Texture quitTexture;
    sf::Texture volumeTexture;
    sf::Texture scoreBackgroundTexture;
    sf::Font font;

    // Sprites
    sf::Sprite backgroundSprite;
    sf::Sprite scoreBackground;

    // Text
    sf::Text soundStateText;
    sf::Text scoreTitle;

    void renderMainMenu(sf::RenderWindow& window);
    void renderScoreboard(sf::RenderWindow& window, const ScoreModel& scoreModel);
    void setupButtonPositions(const sf::RenderWindow& window);
}; 