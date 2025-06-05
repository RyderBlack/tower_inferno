#ifndef TOWER_INFERNO_WAVEMANAGER_H
#define TOWER_INFERNO_WAVEMANAGER_H

#include <vector>
#include <memory>
#include <raylib.h>

// Forward declaration
class GameModel;

struct Wave {
    int waveNumber;
    int enemyCount;
    float spawnRate;
    std::vector<EnemyType> enemyTypes;
    int goldReward;
    bool isBossWave;
};

class WaveManager {
private:
    std::vector<Wave> waves;
    int currentWave;
    bool isWaveInProgress;
    float waveTimer;
    float spawnTimer;
    int enemiesSpawnedThisWave;
    int enemiesKilledThisWave;
    
    // Path that enemies will follow
    std::vector<Vector2> enemyPath;
    
public:
    WaveManager();
    
    void update(float deltaTime, GameModel& gameModel);
    void startNextWave();
    void onEnemyKilled();
    
    // Getters
    int getCurrentWave() const { return currentWave; }
    int getTotalWaves() const { return static_cast<int>(waves.size()); }
    bool getIsWaveInProgress() const { return isWaveInProgress; }
    int getEnemiesRemaining() const { 
        return waves[currentWave].enemyCount - enemiesKilledThisWave; 
    }
    
    // For saving/loading
    void loadWavesFromFile(const char* filename);
    
private:
    void initializeDefaultWaves();
    void spawnEnemy(GameModel& gameModel);
    bool shouldSpawnEnemy() const;
    void onWaveComplete();
};

#endif //TOWER_INFERNO_WAVEMANAGER_H
