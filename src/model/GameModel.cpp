#include "GameModel.h"

GameModel::GameModel() : 
    playerHealth(100), 
    playerGold(500),
    score(0),
    waveNumber(0),
    gameState(GameState::MAIN_MENU) {
    // Initialize wave manager
    waveManager = std::make_unique<WaveManager>();
}

void GameModel::update(float deltaTime) {
    if (gameState != GameState::PLAYING) return;

    // Update all enemies
    for (auto& enemy : enemies) {
        enemy->update(deltaTime);
    }

    // Update all towers
    for (auto& tower : towers) {
        tower->update(deltaTime, enemies);
    }

    // Update all projectiles
    for (auto it = projectiles.begin(); it != projectiles.end();) {
        (*it)->update(deltaTime);
        if ((*it)->shouldRemove()) {
            it = projectiles.erase(it);
        } else {
            ++it;
        }
    }

    // Check for wave completion
    if (enemies.empty() && waveManager->isWaveInProgress()) {
        waveManager->endWave();
        notifyObservers(GameEvent::WAVE_COMPLETE);
    }
}

void GameModel::startGame() {
    gameState = GameState::PLAYING;
    startNextWave();
    notifyObservers(GameEvent::GAME_STARTED);
}

void GameModel::startNextWave() {
    if (waveManager->isWaveInProgress()) return;
    
    waveNumber++;
    auto waveEnemies = waveManager->startWave(waveNumber);
    enemies.insert(enemies.end(), waveEnemies.begin(), waveEnemies.end());
    notifyObservers(GameEvent::WAVE_STARTED);
}

void GameModel::addTower(const Tower& tower) {
    if (playerGold >= tower.getCost()) {
        playerGold -= tower.getCost();
        towers.push_back(std::make_shared<Tower>(tower));
        notifyObservers(GameEvent::TOWER_PLACED);
    }
}

void GameModel::upgradeTower(int towerIndex) {
    if (towerIndex >= 0 && towerIndex < towers.size()) {
        auto& tower = towers[towerIndex];
        if (playerGold >= tower->getUpgradeCost()) {
            playerGold -= tower->getUpgradeCost();
            tower->upgrade();
            notifyObservers(GameEvent::TOWER_UPGRADED);
        }
    }
}

void GameModel::onEnemyReachedEnd(std::shared_ptr<Enemy> enemy) {
    playerHealth -= enemy->getHealth();
    if (playerHealth <= 0) {
        gameState = GameState::GAME_OVER;
        notifyObservers(GameEvent::GAME_OVER);
    }
}

void GameModel::onEnemyDefeated(std::shared_ptr<Enemy> enemy) {
    playerGold += enemy->getBounty();
    score += enemy->getScoreValue();
}

void GameModel::addProjectile(std::shared_ptr<Projectile> projectile) {
    projectiles.push_back(projectile);
}

void GameModel::removeProjectile(std::shared_ptr<Projectile> projectile) {
    projectiles.erase(
        std::remove(projectiles.begin(), projectiles.end(), projectile),
        projectiles.end()
    );
}
