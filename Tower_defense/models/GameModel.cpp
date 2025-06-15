#include "GameModel.h"
#include <algorithm>
#include <cmath>
#include <random>

GameModel::GameModel()
    : state(GameState::MENU)
    , gold(100)
    , lives(10)
    , wave(0)
    , waveTimer(0.0f)
    , spawnTimer(0.0f)
    , spawnInterval(2.0f)
    , waveInProgress(false) {
    initializeGrid();
    initializeEnemyPath();
}

void GameModel::initializeGrid() {
    grid.resize(GRID_HEIGHT, std::vector<int>(GRID_WIDTH, 0));
    
    // Set path tiles
    for (const auto& point : enemyPath) {
        if (point.x >= 0 && point.x < GRID_WIDTH && point.y >= 0 && point.y < GRID_HEIGHT) {
            grid[point.y][point.x] = 1;  // 1 represents path
        }
    }
}

void GameModel::initializeEnemyPath() {
    // Define the path that enemies will follow
    enemyPath = {
        sf::Vector2i(0, 5),
        sf::Vector2i(1, 5),
        sf::Vector2i(2, 5),
        sf::Vector2i(3, 5),
        sf::Vector2i(4, 5),
        sf::Vector2i(5, 5),
        sf::Vector2i(6, 5),
        sf::Vector2i(7, 5),
        sf::Vector2i(8, 5),
        sf::Vector2i(9, 5),
        sf::Vector2i(10, 5),
        sf::Vector2i(11, 5),
        sf::Vector2i(12, 5),
        sf::Vector2i(13, 5),
        sf::Vector2i(14, 5),
        sf::Vector2i(15, 5),
        sf::Vector2i(16, 5),
        sf::Vector2i(17, 5)
    };
}

void GameModel::update(float deltaTime) {
    if (state == GameState::PLAYING) {
        if (waveInProgress) {
            spawnTimer += deltaTime;
            if (spawnTimer >= spawnInterval) {
                spawnEnemy();
                spawnTimer = 0.0f;
            }
        }
        updateTowers(deltaTime);
        updateMonsters(deltaTime);
        checkTowerTargets();
    }
}

void GameModel::spawnEnemy() {
    std::vector<sf::Vector2f> pathPoints;
    for (const auto& point : enemyPath) {
        pathPoints.push_back(sf::Vector2f(point.x * TILE_SIZE + GRID_START_X + TILE_SIZE/2,
                                        point.y * TILE_SIZE + GRID_START_Y + TILE_SIZE/2));
    }
    Path path(pathPoints);
    auto monster = std::make_unique<Monster>(&path, MonsterType::Dolphin);
    monsters.push_back(std::move(monster));
}

void GameModel::handleEnemyDeath(Monster* monster) {
    addGold(10);  // Basic gold reward
    auto it = std::find_if(monsters.begin(), monsters.end(),
                          [monster](const auto& m) { return m.get() == monster; });
    if (it != monsters.end()) {
        monsters.erase(it);
    }
}

void GameModel::handleEnemyReachedEnd(Monster* monster) {
    removeLife();
    auto it = std::find_if(monsters.begin(), monsters.end(),
                          [monster](const auto& m) { return m.get() == monster; });
    if (it != monsters.end()) {
        monsters.erase(it);
    }
}

bool GameModel::placeTower(const sf::Vector2i& pos, TowerType type) {
    if (!isValidGridPosition(pos) || isPathTile(pos)) {
        return false;
    }
    auto tower = TowerFactory::createTower(type, pos);
    if (tower && gold >= tower->cost) {
        removeGold(tower->cost);
        towers.push_back(std::move(tower));
        return true;
    }
    return false;
}

bool GameModel::upgradeTower(const sf::Vector2i& pos) {
    for (auto& tower : towers) {
        if (tower->gridPosition == pos) {
            if (tower->level < tower->maxLevel && gold >= tower->upgradeCost) {
                removeGold(tower->upgradeCost);
                tower->upgrade();
                return true;
            }
        }
    }
    return false;
}

bool GameModel::sellTower(const sf::Vector2i& pos) {
    for (auto it = towers.begin(); it != towers.end(); ++it) {
        if ((*it)->gridPosition == pos) {
            addGold((*it)->getSellPrice());
            towers.erase(it);
            return true;
        }
    }
    return false;
}

void GameModel::startWave() {
    wave++;
    waveInProgress = true;
    spawnTimer = 0.0f;
    spawnInterval = std::max(0.5f, 2.0f - (wave * 0.1f));
}

void GameModel::updateTowers(float deltaTime) {
    for (auto& tower : towers) {
        if (tower->canAttack(waveTimer)) {
            tower->attack(waveTimer);
        }
    }
}

void GameModel::updateMonsters(float deltaTime) {
    for (auto it = monsters.begin(); it != monsters.end();) {
        (*it)->update(deltaTime);
        if ((*it)->hasReachedEnd()) {
            handleEnemyReachedEnd(it->get());
            it = monsters.erase(it);
        } else if (!(*it)->isAlive()) {
            handleEnemyDeath(it->get());
            it = monsters.erase(it);
        } else {
            ++it;
        }
    }
}

void GameModel::checkTowerTargets() {
    for (auto& tower : towers) {
        if (!tower->canAttack(waveTimer)) continue;
        
        Monster* target = nullptr;
        float closestDistance = tower->stats.range;
        
        for (auto& monster : monsters) {
            float distance = std::sqrt(
                std::pow(monster->getPosition().x - tower->position.x, 2) +
                std::pow(monster->getPosition().y - tower->position.y, 2)
            );
            
            if (distance < closestDistance) {
                closestDistance = distance;
                target = monster.get();
            }
        }
        
        if (target) {
            tower->attack(waveTimer);
            target->takeDamage(tower->stats.damage);
        }
    }
}

bool GameModel::isValidGridPosition(const sf::Vector2i& pos) const {
    return pos.x >= 0 && pos.x < GRID_WIDTH && pos.y >= 0 && pos.y < GRID_HEIGHT;
}

bool GameModel::isPathTile(const sf::Vector2i& pos) const {
    return grid[pos.y][pos.x] == 1;
}

void GameModel::addGold(int amount) { 
    gold += amount; 
}

void GameModel::removeGold(int amount) { 
    gold = std::max(0, gold - amount); 
}

void GameModel::removeLife() { 
    lives = std::max(0, lives - 1); 
    if (lives <= 0) {
        state = GameState::GAME_OVER;
    }
}

const std::vector<sf::Vector2i>& GameModel::getEnemyPath() const { 
    return enemyPath; 
}

const std::vector<std::vector<int>>& GameModel::getGrid() const { 
    return grid; 
}