#ifndef TOWER_INFERNO_GAMEMODEL_H
#define TOWER_INFERNO_GAMEMODEL_H

#include <vector>
#include <memory>
#include "../utils/Observable.h"
#include "Enemy.h"
#include "Tower.h"
#include "Projectile.h"
#include "WaveManager.h"

class GameModel : public Observable {
private:
    int playerHealth;
    int playerGold;
    int score;
    bool gameOver;
    bool gameWon;
    
    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Tower>> towers;
    std::vector<std::unique_ptr<Projectile>> projectiles;
    
    WaveManager waveManager;
    
public:
    GameModel();
    
    // Game state management
    void update(float deltaTime);
    void placeTower(const Vector2& position, TowerType type);
    void sellTower(int towerIndex);
    void upgradeTower(int towerIndex);
    
    // Getters
    int getPlayerHealth() const { return playerHealth; }
    int getPlayerGold() const { return playerGold; }
    int getScore() const { return score; }
    bool isGameOver() const { return gameOver; }
    bool isGameWon() const { return gameWon; }
    
    // Getters for containers (returning const references to prevent modification)
    const std::vector<std::unique_ptr<Enemy>>& getEnemies() const { return enemies; }
    const std::vector<std::unique_ptr<Tower>>& getTowers() const { return towers; }
    const std::vector<std::unique_ptr<Projectile>>& getProjectiles() const { return projectiles; }
    const WaveManager& getWaveManager() const { return waveManager; }
    
    // Helper functions
    void addGold(int amount);
    void takeDamage(int amount);
    void addScore(int points);
    
private:
    void spawnWave();
    void spawnEnemy(EnemyType type);
    void updateTowers(float deltaTime);
    void updateProjectiles(float deltaTime);
    void updateEnemies(float deltaTime);
    void checkCollisions();
    void checkWaveCompletion();
    void endGame(bool won);
};

#endif //TOWER_INFERNO_GAMEMODEL_H
