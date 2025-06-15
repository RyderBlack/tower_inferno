#include "MenuView.h"
#include <iostream>
#include <sstream>
#include "../ResourcePath.h"
#include "../GameConstants.h"

MenuView::MenuView() {
    // Load all resources first
    loadResources();
    
    // Set up button sprites
    playButton.setTexture(playTexture);
    scoreButton.setTexture(scoreTexture);
    quitButton.setTexture(quitTexture);
    volumeButton.setTexture(volumeTexture);
    backgroundSprite.setTexture(backgroundTexture);
    scoreBackground.setTexture(scoreBackgroundTexture);
    
    // Set up text elements after font is loaded
    soundStateText.setFont(font);
    soundStateText.setCharacterSize(24);
    soundStateText.setFillColor(sf::Color::White);
    soundStateText.setString("Sound: On");
    
    scoreTitle.setFont(font);
    scoreTitle.setCharacterSize(48);
    scoreTitle.setFillColor(sf::Color::White);
    scoreTitle.setString("High Scores");
    
    scoreBackButton.setFont(font);
    scoreBackButton.setCharacterSize(24);
    scoreBackButton.setFillColor(sf::Color::White);
    scoreBackButton.setString("Back to Menu");
    
    // Set initial positions
    setupButtonPositions(sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), ""));
    
    std::cout << "MenuView initialized successfully" << std::endl;
}

void MenuView::loadResources() {
    std::cout << "Loading menu resources..." << std::endl;
    
    // Load font first as it's needed for text elements
    std::string fontPath = getAssetPath("fonts/arial.ttf");
    std::cout << "Loading font from: " << fontPath << std::endl;
    if (!font.loadFromFile(fontPath)) {
        std::cerr << "Failed to load font from " << fontPath << ", trying fallback..." << std::endl;
        if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
            std::cerr << "Failed to load fallback font" << std::endl;
        } else {
            std::cout << "Successfully loaded fallback font" << std::endl;
        }
    } else {
        std::cout << "Successfully loaded font" << std::endl;
    }
    
    // Load background
    std::string bgPath = getAssetPath("image/tower_inferno_fon.png");
    std::cout << "Loading background from: " << bgPath << std::endl;
    if (!backgroundTexture.loadFromFile(bgPath)) {
        std::cerr << "Failed to load background from " << bgPath << std::endl;
    } else {
        std::cout << "Successfully loaded background texture" << std::endl;
    }
    
    // Load button textures
    std::string playPath = getAssetPath("image/play_button.png");
    std::cout << "Loading play button from: " << playPath << std::endl;
    if (!playTexture.loadFromFile(playPath)) {
        std::cerr << "Failed to load play button from " << playPath << std::endl;
    } else {
        std::cout << "Successfully loaded play button" << std::endl;
    }
    
    std::string scorePath = getAssetPath("image/score_button.png");
    std::cout << "Loading score button from: " << scorePath << std::endl;
    if (!scoreTexture.loadFromFile(scorePath)) {
        std::cerr << "Failed to load score button from " << scorePath << std::endl;
    } else {
        std::cout << "Successfully loaded score button" << std::endl;
    }
    
    std::string quitPath = getAssetPath("image/quit_icon.png");
    std::cout << "Loading quit icon from: " << quitPath << std::endl;
    if (!quitTexture.loadFromFile(quitPath)) {
        std::cerr << "Failed to load quit icon from " << quitPath << std::endl;
    } else {
        std::cout << "Successfully loaded quit icon" << std::endl;
    }
    
    std::string volumePath = getAssetPath("image/volume_icon.png");
    std::cout << "Loading volume icon from: " << volumePath << std::endl;
    if (!volumeTexture.loadFromFile(volumePath)) {
        std::cerr << "Failed to load volume icon from " << volumePath << std::endl;
    } else {
        std::cout << "Successfully loaded volume icon" << std::endl;
    }
    
    // Use the same background for score background for now
    std::string scoreBgPath = getAssetPath("image/tower_inferno_fon.png");
    std::cout << "Loading score background from: " << scoreBgPath << std::endl;
    if (!scoreBackgroundTexture.loadFromFile(scoreBgPath)) {
        std::cerr << "Failed to load score background from " << scoreBgPath << std::endl;
    } else {
        std::cout << "Successfully loaded score background" << std::endl;
    }
    
    std::cout << "Finished loading menu resources" << std::endl;
}

void MenuView::render(sf::RenderWindow& window, const GameModel& model, const ScoreModel& scoreModel) {
    // Draw background
    window.draw(backgroundSprite);

    if (model.getState() == GameModel::GameState::SCORE) {
        renderScoreboard(window, scoreModel);
    } else {
        renderMainMenu(window);
    }
}

void MenuView::renderMainMenu(sf::RenderWindow& window) {
    window.draw(playButton);
    window.draw(scoreButton);
    window.draw(quitButton);
    window.draw(volumeButton);
    window.draw(soundStateText);
}

void MenuView::renderScoreboard(sf::RenderWindow& window, const ScoreModel& scoreModel) {
    // Draw scoreboard background
    window.draw(scoreBackground);

    // Draw title
    sf::FloatRect titleBounds = scoreTitle.getLocalBounds();
    scoreTitle.setPosition(
        (WINDOW_WIDTH - titleBounds.width) / 2,
        100
    );
    window.draw(scoreTitle);

    // Draw scores
    auto scores = scoreModel.loadScores();
    scoreModel.sortScores(scores);

    for (size_t i = 0; i < std::min(scores.size(), size_t(10)); ++i) {
        sf::Text scoreText;
        scoreText.setFont(font);
        scoreText.setCharacterSize(24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setString(std::to_string(i + 1) + ". " + std::to_string(scores[i]));

        sf::FloatRect bounds = scoreText.getLocalBounds();
        scoreText.setPosition(
            (WINDOW_WIDTH - bounds.width) / 2,
            200 + i * 40
        );
        window.draw(scoreText);
    }

    // Draw back button
    window.draw(scoreBackButton);
}

void MenuView::setupButtonPositions(const sf::RenderWindow& window) {
    const float centerX = window.getSize().x / 2.0f;
    const float startY = window.getSize().y / 3.0f;
    
    // Scale buttons to fit the window size
    const float buttonScale = std::min(
        window.getSize().x / 800.0f,  // Base width of 800
        window.getSize().y / 600.0f    // Base height of 600
    );
    
    // Scale buttons
    playButton.setScale(buttonScale, buttonScale);
    scoreButton.setScale(buttonScale, buttonScale);
    quitButton.setScale(buttonScale, buttonScale);
    volumeButton.setScale(buttonScale * 0.5f, buttonScale * 0.5f);
    
    // Position buttons with proper spacing
    float buttonSpacing = 80.0f * buttonScale;
    
    // Play button
    sf::FloatRect bounds = playButton.getLocalBounds();
    playButton.setOrigin(bounds.width / 2, bounds.height / 2);
    playButton.setPosition(centerX, startY);

    // Score button
    bounds = scoreButton.getLocalBounds();
    scoreButton.setOrigin(bounds.width / 2, bounds.height / 2);
    scoreButton.setPosition(centerX, startY + buttonSpacing);

    // Quit button
    bounds = quitButton.getLocalBounds();
    quitButton.setOrigin(bounds.width / 2, bounds.height / 2);
    quitButton.setPosition(centerX, startY + 2 * buttonSpacing);

    // Volume button (top-left corner)
    bounds = volumeButton.getLocalBounds();
    volumeButton.setOrigin(0, 0);
    float padding = 20.0f * buttonScale;
    volumeButton.setPosition(padding, padding);

    // Sound state text (next to volume button)
    soundStateText.setCharacterSize(static_cast<unsigned int>(24 * buttonScale));
    soundStateText.setPosition(volumeButton.getPosition().x + bounds.width + 10.0f, 
                              volumeButton.getPosition().y + (bounds.height - soundStateText.getLocalBounds().height) / 2.0f);
    
    // Score background (centered)
    scoreBackground.setScale(buttonScale, buttonScale);
    bounds = scoreBackground.getLocalBounds();
    scoreBackground.setOrigin(bounds.width / 2, bounds.height / 2);
    scoreBackground.setPosition(centerX, window.getSize().y / 2.0f);

    // Score title (centered above the score background)
    scoreTitle.setCharacterSize(static_cast<unsigned int>(48 * buttonScale));
    bounds = scoreTitle.getLocalBounds();
    scoreTitle.setOrigin(bounds.width / 2, bounds.height / 2);
    scoreTitle.setPosition(centerX, scoreBackground.getPosition().y - bounds.height - 20.0f);

    // Back button (centered below the score background)
    scoreBackButton.setCharacterSize(static_cast<unsigned int>(24 * buttonScale));
    bounds = scoreBackButton.getLocalBounds();
    scoreBackButton.setOrigin(bounds.width / 2, bounds.height / 2);
    scoreBackButton.setPosition(centerX, scoreBackground.getPosition().y + scoreBackground.getLocalBounds().height / 2 + 60.0f);
}

void MenuView::setSoundState(bool enabled) {
    soundStateText.setString(enabled ? "Sound: On" : "Sound: Off");
}