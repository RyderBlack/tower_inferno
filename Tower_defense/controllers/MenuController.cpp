#include "MenuController.h"

MenuController::MenuController(GameModel& model, MenuView& view)
    : model(model)
    , view(view)
    , soundEnabled(true) {
}

void MenuController::handleInput(sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(
                sf::Vector2i(event.mouseButton.x, event.mouseButton.y)
            );
            handleMouseClick(mousePos);
        }
    }
}

void MenuController::handleMouseClick(const sf::Vector2f& mousePos) {
    if (model.getState() == GameModel::GameState::SCORE) {
        // Check if back button was clicked
        if (view.scoreBackButton.getGlobalBounds().contains(mousePos)) {
            model.setState(GameModel::GameState::MENU);
        }
    } else {
        // Check main menu buttons
        if (view.playButton.getGlobalBounds().contains(mousePos)) {
            model.setState(GameModel::GameState::PLAYING);
        } else if (view.scoreButton.getGlobalBounds().contains(mousePos)) {
            model.setState(GameModel::GameState::SCORE);
        } else if (view.quitButton.getGlobalBounds().contains(mousePos)) {
            // Handle quit button click
            // This should be handled by the main window
        } else if (view.volumeButton.getGlobalBounds().contains(mousePos)) {
            toggleSound();
        }
    }
}

void MenuController::toggleSound() {
    soundEnabled = !soundEnabled;
    view.setSoundState(soundEnabled);
    // TODO: Implement actual sound toggling
} 