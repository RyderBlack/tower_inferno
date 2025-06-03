#pragma once
#include "Tower.h"
#include "Enemy.h"
#include <vector>
#include <memory>

class Game {
public:
    enum class State { MENU, PLAYING, GAME_OVER };
    
    Game();
    
    void Update(float deltaTime);
    void SpawnWave();
    void StartGame();
    void Reset();
    
    // Getters
    State GetState() const;
    int GetScore() const;
    int GetWave() const;
    bool IsSoundOn() const;
    bool IsGameWon() const;
    const std::vector<std::unique_ptr<Enemy>>& GetEnemies() const;
    const Tower& GetTower() const;
    
    // Input handling
    void ToggleSound();
    
private:
    State state;
    int score;
    int wave;
    bool soundOn;
    bool gameWon;
    float waveTimer;
    int enemiesPerWave;
    
    Tower tower;
    std::vector<std::unique_ptr<Enemy>> enemies;
    
    void CheckGameOver();
};
