#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>
#include <chrono>
#include "../tower.h"
#include "../monster.h"

class GameModel {
public:
    enum class GameState { MENU, PLAYING, GAME_OVER, SCORE };

    // Constants
    static constexpr int TILE_SIZE = 50;
    static constexpr int GRID_WIDTH = 30;
    static constexpr int GRID_HEIGHT = 20;
    static constexpr int GRID_START_X = 100;
    static constexpr int GRID_START_Y = 100;

    // Tile types
    static constexpr int TILE_ROCK = 0;
    static constexpr int HORIZONTALE_LAVA = 1;
    static constexpr int VERTICALE_LAVA = 2;
    static constexpr int VIRAGE_GAUCHE_BAS = 3;
    static constexpr int VIRAGE_HAUT_DROITE = 4;
    static constexpr int VIRAGE_GAUCHE_HAUT = 5;
    static constexpr int VIRAGE_BAS_DROITE = 6;

    GameModel();
    void update(float deltaTime);
    void startWave();
    void handleEnemyDeath(Monster* enemy);
    void handleEnemyReachedEnd(Monster* enemy);
    bool placeTower(const sf::Vector2i& pos, TowerType type);
    bool upgradeTower(const sf::Vector2i& pos);
    bool sellTower(const sf::Vector2i& pos);
    bool isValidGridPosition(const sf::Vector2i& pos) const;
    bool isPathTile(const sf::Vector2i& pos) const;
    const std::vector<sf::Vector2i>& getEnemyPath() const;

    // Getters
    GameState getState() const { return state; }
    int getLives() const { return lives; }
    int getGold() const { return gold; }
    int getWave() const { return wave; }
    const std::vector<std::vector<int>>& getGrid() const;
    const std::vector<std::unique_ptr<Tower>>& getTowers() const { return towers; }
    const std::vector<std::unique_ptr<Monster>>& getMonsters() const { return monsters; }
    bool isWaveInProgress() const { return waveInProgress; }
    float getWaveTimer() const { return waveTimer; }

    // Setters
    void setState(GameState newState) { state = newState; }
    void setLives(int newLives) { lives = newLives; }
    void setGold(int newGold) { gold = newGold; }
    void setWave(int newWave) { wave = newWave; }
    void addGold(int amount);
    void removeGold(int amount);
    void removeLife();

private:
    GameState state;
    int lives;
    int gold;
    int wave;
    float waveTimer;
    float spawnTimer;
    float spawnInterval;
    bool waveInProgress;
    std::vector<std::vector<int>> grid;
    std::vector<sf::Vector2i> enemyPath;
    std::vector<std::unique_ptr<Tower>> towers;
    std::vector<std::unique_ptr<Monster>> monsters;

    void initializeGrid();
    void initializeEnemyPath();
    void updateTowers(float deltaTime);
    void updateMonsters(float deltaTime);
    void checkTowerTargets();
    void spawnEnemy();
    void checkWaveEnd();
}; 