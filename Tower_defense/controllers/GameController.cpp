#include "GameController.h"
#include "../tower.h"

GameController::GameController(GameModel& model, GameView& view)
    : model(model)
    , view(view)
    , menuJustOpened(false)
    , selectedTowerIndex(-1)
    , selectedGridPos(-1, -1) {
}

void GameController::handleInput(sf::Event& event, sf::RenderWindow& window) {
    if (event.type == sf::Event::MouseButtonPressed) {
        if (event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos = window.mapPixelToCoords(
                sf::Vector2i(event.mouseButton.x, event.mouseButton.y)
            );
            handleMouseClick(mousePos);
        } else if (event.key.code == sf::Keyboard::Escape) {
            model.setState(GameModel::GameState::MENU);
        }
    }
}

void GameController::handleMouseClick(const sf::Vector2f& mousePos) {
    // Convert mouse position to grid coordinates
    sf::Vector2i gridPos(
        static_cast<int>((mousePos.x - GRID_START_X) / TILE_SIZE),
        static_cast<int>((mousePos.y - GRID_START_Y) / TILE_SIZE)
    );

    // Check if click is within grid bounds
    if (gridPos.x >= 0 && gridPos.x < GRID_WIDTH &&
        gridPos.y >= 0 && gridPos.y < GRID_HEIGHT) {
        handleTowerPlacement(gridPos);
    }
}

void GameController::handleTowerPlacement(const sf::Vector2i& gridPos) {
    if (model.isValidGridPosition(gridPos) && !model.isPathTile(gridPos)) {
        model.placeTower(gridPos, TowerType::BASIC);
    }
}

void GameController::handleTowerUpgrade(size_t towerIndex) {
    // Use selectedGridPos instead of towerIndex
    model.upgradeTower(selectedGridPos);
}

void GameController::handleTowerSell(size_t towerIndex) {
    // Use selectedGridPos instead of towerIndex
    model.sellTower(selectedGridPos);
} 