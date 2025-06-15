#include "GameView.h"
#include <iostream>
#include <fstream>
#include "../ResourcePath.h"

GameView::GameView() {
    loadResources();
}

void GameView::loadResources() {
    std::cout << "\n=== Loading Game Resources ===" << std::endl;
    
    // Helper function to load textures with detailed error reporting
    auto loadTexture = [](const std::string& path, sf::Texture& texture, const std::string& name) -> bool {
        std::string fullPath = getAssetPath(path);
        std::cout << "Loading " << name << " from: " << fullPath << std::endl;
        
        // Check if file exists
        std::ifstream file(fullPath);
        if (!file.good()) {
            std::cerr << "  Error: File does not exist or is not accessible" << std::endl;
            return false;
        }
        file.close();
        
        // Try to load the texture
        if (!texture.loadFromFile(fullPath)) {
            std::cerr << "  Error: Failed to load texture" << std::endl;
            return false;
        }
        
        std::cout << "  Success: Loaded " << name << " (" << texture.getSize().x << "x" 
                  << texture.getSize().y << ")" << std::endl;
        return true;
    };
    
    // Load game textures with error tracking
    bool allTexturesLoaded = true;
    allTexturesLoaded &= loadTexture("sprites/rock.png", rockTexture, "rock");
    allTexturesLoaded &= loadTexture("sprites/lava1.png", lavaTexture1, "lava1");
    allTexturesLoaded &= loadTexture("sprites/lava2.png", lavaTexture2, "lava2");
    allTexturesLoaded &= loadTexture("sprites/lava3.png", lavaTexture3, "lava3");
    allTexturesLoaded &= loadTexture("sprites/lava4.png", lavaTexture4, "lava4");
    allTexturesLoaded &= loadTexture("sprites/lava5.png", lavaTexture5, "lava5");
    allTexturesLoaded &= loadTexture("sprites/lava6.png", lavaTexture6, "lava6");
    allTexturesLoaded &= loadTexture("sprites/gold.png", goldTexture, "gold");
    allTexturesLoaded &= loadTexture("sprites/heart.png", heartTexture, "heart");
    allTexturesLoaded &= loadTexture("sprites/background.png", backgroundTexture, "background");
    
    // Load font with multiple fallbacks
    std::vector<std::string> fontPaths = {
        getAssetPath("fonts/arial.ttf"),
        "C:/Windows/Fonts/arial.ttf",
        "C:/Windows/Fonts/calibri.ttf",
        "C:/Windows/Fonts/times.ttf"
    };
    
    bool fontLoaded = false;
    for (size_t i = 0; i < fontPaths.size() && !fontLoaded; ++i) {
        std::cout << "Attempting to load font from: " << fontPaths[i] << std::endl;
        if (font.loadFromFile(fontPaths[i])) {
            std::cout << "  Success: Loaded font from " << fontPaths[i] << std::endl;
            fontLoaded = true;
        } else {
            std::cerr << "  Failed to load font from " << fontPaths[i] << std::endl;
        }
    }
    
    if (!fontLoaded) {
        std::cerr << "Error: Could not load any font! Text will not be rendered." << std::endl;
    }
    
    // Initialize UI text elements
    if (fontLoaded) {
        goldText.setFont(font);
        goldText.setCharacterSize(24);
        goldText.setFillColor(sf::Color::White);
        goldText.setPosition(10, 10);
        
        livesText.setFont(font);
        livesText.setCharacterSize(24);
        livesText.setFillColor(sf::Color::White);
        livesText.setPosition(10, 40);
        
        waveText.setFont(font);
        waveText.setCharacterSize(24);
        waveText.setFillColor(sf::Color::White);
        waveText.setPosition(10, 70);
    }
    
    std::cout << "=== Finished loading game resources ===" << std::endl;
    if (!allTexturesLoaded) {
        std::cerr << "Warning: Some textures failed to load!" << std::endl;
    }
}

void GameView::handleResize(const sf::Vector2u& newSize) {
    // Update the game view to maintain aspect ratio
    sf::FloatRect viewport(0.f, 0.f, 1.f, 1.f);
    
    // Calculate aspect ratios
    float windowAspect = static_cast<float>(newSize.x) / newSize.y;
    float gameAspect = static_cast<float>(GRID_WIDTH * TILE_SIZE) / (GRID_HEIGHT * TILE_SIZE);
    
    if (windowAspect > gameAspect) {
        // Window is wider than game aspect ratio
        float viewWidth = gameAspect / windowAspect;
        float viewX = (1.0f - viewWidth) / 2.0f;
        viewport = sf::FloatRect(viewX, 0.0f, viewWidth, 1.0f);
    } else {
        // Window is taller than game aspect ratio
        float viewHeight = windowAspect / gameAspect;
        float viewY = (1.0f - viewHeight) / 2.0f;
        viewport = sf::FloatRect(0.0f, viewY, 1.0f, viewHeight);
    }
    
    // Update the game view
    gameView.setViewport(viewport);
    
    // Update UI view to always match the window size exactly
    uiView = sf::View(sf::FloatRect(0, 0, newSize.x, newSize.y));
    
    // Update text positions if needed
    if (font.getInfo().family != "") {
        goldText.setPosition(10, 10);
        livesText.setPosition(10, 40);
        waveText.setPosition(10, 70);
    }
}

void GameView::render(sf::RenderWindow& window, const GameModel& model) {
    static sf::Clock frameClock;
    static int frameCount = 0;
    static float lastFpsUpdate = 0;
    static float fps = 0;
    
    // Calculate FPS
    frameCount++;
    float currentTime = frameClock.getElapsedTime().asSeconds();
    if (currentTime - lastFpsUpdate >= 1.0f) {
        fps = frameCount / (currentTime - lastFpsUpdate);
        frameCount = 0;
        lastFpsUpdate = currentTime;
    }
    
    // Clear the window
    window.clear(sf::Color(30, 30, 40));
    
    // Draw background
    if (backgroundTexture.getSize().x > 0 && backgroundTexture.getSize().y > 0) {
        sf::Sprite background(backgroundTexture);
        // Scale background to fit window
        float scaleX = static_cast<float>(window.getSize().x) / background.getLocalBounds().width;
        float scaleY = static_cast<float>(window.getSize().y) / background.getLocalBounds().height;
        background.setScale(scaleX, scaleY);
        window.draw(background);
    } else {
        // Fallback background if texture failed to load
        sf::RectangleShape background(sf::Vector2f(window.getSize().x, window.getSize().y));
        background.setFillColor(sf::Color(40, 44, 52));
        window.draw(background);
    }
    
    // Save the current view
    sf::View currentView = window.getView();
    
    // Set up game view (for grid, towers, enemies)
    sf::View gameView(sf::FloatRect(0, 0, window.getSize().x, window.getSize().y));
    gameView.setViewport(sf::FloatRect(0, 0, 1.0f, 1.0f));
    window.setView(gameView);
    
    // Draw game elements
    renderGrid(window, model);
    renderTowers(window, model);
    renderEnemies(window, model);
    
    // Reset to default view for UI elements
    window.setView(window.getDefaultView());
    
    // Draw UI
    renderUI(window, model);
    
    // Draw debug info
    if (showDebugInfo) {
        sf::Text debugText;
        debugText.setFont(font);
        debugText.setCharacterSize(14);
        debugText.setFillColor(sf::Color::White);
        debugText.setPosition(10, window.getSize().y - 60);
        
        std::stringstream ss;
        ss << "FPS: " << static_cast<int>(fps) << "\n"
           << "Mouse: (" << sf::Mouse::getPosition(window).x 
           << ", " << sf::Mouse::getPosition(window).y << ")\n"
           << "Enemies: " << model.getMonsters().size() << "\n"
           << "Towers: " << model.getTowers().size();
           
        debugText.setString(ss.str());
        window.draw(debugText);
    }
    
    // Restore the original view
    window.setView(currentView);
}

void GameView::renderGrid(sf::RenderWindow& window, const GameModel& model) {
    const auto& grid = model.getGrid();
    for (int y = 0; y < GRID_HEIGHT; ++y) {
        for (int x = 0; x < GRID_WIDTH; ++x) {
            sf::Sprite tileSprite;
            if (grid[y][x] == 1) {  // Path tile
                tileSprite.setTexture(lavaTexture1);
            } else {  // Empty tile
                tileSprite.setTexture(rockTexture);
            }
            tileSprite.setPosition(GRID_START_X + x * TILE_SIZE, GRID_START_Y + y * TILE_SIZE);
            window.draw(tileSprite);
        }
    }
}

void GameView::renderTowers(sf::RenderWindow& window, const GameModel& model) {
    for (const auto& tower : model.getTowers()) {
        tower->render(window, TILE_SIZE);
    }
}

void GameView::renderEnemies(sf::RenderWindow& window, const GameModel& model) {
    for (const auto& monster : model.getMonsters()) {
        monster->draw(window);
    }
}

void GameView::renderUI(sf::RenderWindow& window, const GameModel& model) {
    // Draw gold
    goldText.setString("Gold: " + std::to_string(model.getGold()));
    goldText.setPosition(WINDOW_WIDTH - UI_WIDTH + 20, 20);
    window.draw(goldText);

    // Draw wave info
    waveText.setString("Wave: " + std::to_string(model.getWave()));
    waveText.setPosition(WINDOW_WIDTH - UI_WIDTH + 20, 60);
    window.draw(waveText);

    // Draw lives
    livesText.setString("Lives: " + std::to_string(model.getLives()));
    livesText.setPosition(WINDOW_WIDTH - UI_WIDTH + 20, 100);
    window.draw(livesText);
} 