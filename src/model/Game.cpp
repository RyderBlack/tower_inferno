#include "Game.h"
#include <algorithm>

Game::Game()
    : state(State::MENU)
    , score(0)
    , wave(1)
    , soundOn(true)
    , gameWon(false)
    , waveTimer(0)
    , enemiesPerWave(5)
    , tower({0, 0.5f, 0}, 4.0f, 1.0f) {}

void Game::Update(float deltaTime) {
    if (state != State::PLAYING) return;
    
    // Update enemies
    for (auto& enemy : enemies) {
        if (enemy->IsAlive()) {
            enemy->Update(deltaTime);
        }
    }
    
    // Remove dead enemies and check if any reached the end
    enemies.erase(
        std::remove_if(enemies.begin(), enemies.end(),
            [this](const std::unique_ptr<Enemy>& e) {
                if (!e->IsAlive()) {
                    score += 20; // Reward for killing
                    return true;
                }
                return false;
            }),
        enemies.end()
    );
    
    // Update tower
    std::vector<Enemy*> aliveEnemies;
    for (auto& enemy : enemies) {
        if (enemy->IsAlive()) {
            aliveEnemies.push_back(enemy.get());
        }
    }
    tower.Update(deltaTime, aliveEnemies);
    
    // Check for wave completion
    if (enemies.empty()) {
        wave++;
        enemiesPerWave++;
        if (wave > 5) {
            gameWon = true;
            state = State::GAME_OVER;
        } else {
            SpawnWave();
        }
    }
    
    CheckGameOver();
}

void Game::SpawnWave() {
    for (int i = 0; i < enemiesPerWave; ++i) {
        enemies.push_back(std::make_unique<Enemy>(
            Vector3{-8.0f, 0.3f, -5.0f + i * 2.0f}, 
            1.2f, 
            30.0f
        ));
    }
}

void Game::StartGame() {
    Reset();
    state = State::PLAYING;
    SpawnWave();
}

void Game::Reset() {
    score = 0;
    wave = 1;
    enemiesPerWave = 5;
    gameWon = false;
    enemies.clear();
}

void Game::CheckGameOver() {
    if (score < -50) {
        gameWon = false;
        state = State::GAME_OVER;
    }
}

// Getters
Game::State Game::GetState() const { return state; }
int Game::GetScore() const { return score; }
int Game::GetWave() const { return wave; }
bool Game::IsSoundOn() const { return soundOn; }
bool Game::IsGameWon() const { return gameWon; }
const std::vector<std::unique_ptr<Enemy>>& Game::GetEnemies() const { return enemies; }
const Tower& Game::GetTower() const { return tower; }

// Input handling
void Game::ToggleSound() { soundOn = !soundOn; }
