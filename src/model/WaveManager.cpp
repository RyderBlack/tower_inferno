#include "WaveManager.h"
#include <random>
#include <algorithm>

WaveManager::WaveManager() : currentWave(0), waveInProgress(false), timeUntilNextWave(0.0f) {
    // Initialize path (this would typically be loaded from a level file)
    initializeDefaultPath();
    
    // Initialize enemy templates
    initializeEnemyTemplates();
}

std::vector<std::shared_ptr<Enemy>> WaveManager::startWave(int waveNumber) {
    currentWave = waveNumber;
    waveInProgress = true;
    timeUntilNextWave = 0.0f;
    
    std::vector<std::shared_ptr<Enemy>> newEnemies;
    int enemyCount = calculateEnemyCount(waveNumber);
    
    // Create enemies for this wave
    for (int i = 0; i < enemyCount; i++) {
        // Choose enemy type based on wave number and random chance
        EnemyType type = chooseEnemyType(waveNumber);
        const auto& templateEnemy = enemyTemplates[type];
        
        // Create enemy with scaled stats based on wave number
        float health = templateEnemy.health * (1.0f + (waveNumber - 1) * 0.2f);
        float speed = templateEnemy.speed * (1.0f + (waveNumber - 1) * 0.05f);
        int bounty = static_cast<int>(templateEnemy.bounty * (1.0f + (waveNumber - 1) * 0.1f));
        int scoreValue = static_cast<int>(templateEnemy.scoreValue * (1.0f + (waveNumber - 1) * 0.1f));
        
        auto enemy = std::make_shared<Enemy>(
            type, path, health, speed, bounty, scoreValue
        );
        
        // Stagger enemy spawns
        enemy->setSpawnDelay(i * 0.5f);
        
        newEnemies.push_back(enemy);
    }
    
    return newEnemies;
}

void WaveManager::endWave() {
    waveInProgress = false;
    timeUntilNextWave = 30.0f; // 30 seconds until next wave starts automatically
}

void WaveManager::update(float deltaTime) {
    if (!waveInProgress) {
        timeUntilNextWave = std::max(0.0f, timeUntilNextWave - deltaTime);
    }
}

bool WaveManager::isWaveInProgress() const {
    return waveInProgress;
}

int WaveManager::getCurrentWave() const {
    return currentWave;
}

float WaveManager::getTimeUntilNextWave() const {
    return timeUntilNextWave;
}

void WaveManager::initializeDefaultPath() {
    // Create a simple default path (in a real game, this would be loaded from a level file)
    path = {
        { -50.0f, 300.0f },   // Start off-screen left
        { 100.0f, 300.0f },   // Enter from left
        { 100.0f, 100.0f },   // Move up
        { 500.0f, 100.0f },   // Move right
        { 500.0f, 500.0f },   // Move down
        { 900.0f, 500.0f },   // Move right
        { 900.0f, 300.0f },   // Move up
        { 1200.0f, 300.0f }   // Exit off-screen right
    };
}

void WaveManager::initializeEnemyTemplates() {
    // Basic enemy
    enemyTemplates[EnemyType::BASIC] = {
        .health = 100.0f,
        .speed = 100.0f,
        .bounty = 10,
        .scoreValue = 10
    };
    
    // Fast enemy
    enemyTemplates[EnemyType::FAST] = {
        .health = 50.0f,
        .speed = 200.0f,
        .bounty = 15,
        .scoreValue = 15
    };
    
    // Tank enemy
    enemyTemplates[EnemyType::TANK] = {
        .health = 400.0f,
        .speed = 60.0f,
        .bounty = 30,
        .scoreValue = 30
    };
    
    // Flying enemy (ignores path, moves directly)
    enemyTemplates[EnemyType::FLYING] = {
        .health = 75.0f,
        .speed = 120.0f,
        .bounty = 20,
        .scoreValue = 20
    };
    
    // Boss enemy
    enemyTemplates[EnemyType::BOSS] = {
        .health = 2000.0f,
        .speed = 80.0f,
        .bounty = 100,
        .scoreValue = 100
    };
}

int WaveManager::calculateEnemyCount(int waveNumber) const {
    // Increase enemy count with each wave
    return 5 + waveNumber * 2;
}

EnemyType WaveManager::chooseEnemyType(int waveNumber) const {
    // Simple random enemy type selection based on wave number
    std::random_device rd;
    std::mt19937 gen(rd());
    
    // Adjust probabilities based on wave number
    if (waveNumber < 3) {
        // Early waves: only basic enemies
        return EnemyType::BASIC;
    } else if (waveNumber < 6) {
        // Introduce fast enemies
        std::uniform_int_distribution<> dis(0, 3);
        int roll = dis(gen);
        return (roll == 0) ? EnemyType::FAST : EnemyType::BASIC;
    } else if (waveNumber < 10) {
        // Introduce tank enemies
        std::uniform_int_distribution<> dis(0, 9);
        int roll = dis(gen);
        if (roll < 2) return EnemyType::TANK;
        if (roll < 5) return EnemyType::FAST;
        return EnemyType::BASIC;
    } else {
        // All enemy types possible
        std::uniform_int_distribution<> dis(0, 99);
        int roll = dis(gen);
        
        if (waveNumber % 10 == 0) {
            // Boss wave
            return EnemyType::BOSS;
        } else if (roll < 10) {
            return EnemyType::TANK;
        } else if (roll < 30) {
            return EnemyType::FAST;
        } else if (roll < 40 && waveNumber > 15) {
            return EnemyType::FLYING;
        } else {
            return EnemyType::BASIC;
        }
    }
}
