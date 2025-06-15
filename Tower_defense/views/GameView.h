#ifndef GAME_VIEW_H
#define GAME_VIEW_H

#include <SFML/Graphics.hpp>
#include <sstream>
#include "../models/GameModel.h"
#include "../GameConstants.h"

class GameView {
public:
    GameView();
    
    // Main rendering function
    void render(sf::RenderWindow& window, const GameModel& model);
    
    // Toggle debug information display
    void toggleDebugInfo() { showDebugInfo = !showDebugInfo; }
    
    // Handle window resize
    void handleResize(const sf::Vector2u& newSize);

private:
    // Resource management
    void loadResources();
    
    // Rendering functions
    void renderGrid(sf::RenderWindow& window, const GameModel& model);
    void renderTowers(sf::RenderWindow& window, const GameModel& model);
    void renderEnemies(sf::RenderWindow& window, const GameModel& model);
    void renderUI(sf::RenderWindow& window, const GameModel& model);

    // Game textures
    sf::Texture rockTexture;
    sf::Texture lavaTexture1;
    sf::Texture lavaTexture2;
    sf::Texture lavaTexture3;
    sf::Texture lavaTexture4;
    sf::Texture lavaTexture5;
    sf::Texture lavaTexture6;
    sf::Texture goldTexture;
    sf::Texture heartTexture;
    sf::Texture backgroundTexture;
    
    // Fonts
    sf::Font font;

    // UI Elements
    sf::Text goldText;
    sf::Text waveText;
    sf::Text livesText;
    
    // View management
    sf::View gameView;  // For game world rendering
    sf::View uiView;    // For UI elements
    
    // Debug settings
    bool showDebugInfo = true;  // Toggle with F3 key
    
    // Helper function to format numbers with commas
    std::string formatNumber(int number) const {
        std::string numStr = std::to_string(number);
        int insertPosition = numStr.length() - 3;
        while (insertPosition > 0) {
            numStr.insert(insertPosition, ",");
            insertPosition -= 3;
        }
        return numStr;
    }
};

#endif // GAME_VIEW_H