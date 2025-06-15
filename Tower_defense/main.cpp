#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include <iostream>
#include <random>
#include <chrono>
#include <fstream>
#include "tower.h"
#include "include/json.hpp"
#include "models/GameModel.h"
#include "models/ScoreModel.h"
#include "views/GameView.h"
#include "views/MenuView.h"
#include "controllers/GameController.h"
#include "controllers/MenuController.h"
#include "GameConstants.h"
#include "ResourcePath.h"

using json = nlohmann::json;

// Game states
enum class GameState { MENU, SCORE, PLAYING };

// JSON helpers
void saveScoreToJson(int newScore) {
    json data;
    std::ifstream inFile("menu/score.json");
    if (inFile.is_open()) {
        inFile >> data;
        inFile.close();
    }
    data["scores"].push_back(newScore);
    std::ofstream outFile("menu/score.json");
    outFile << data.dump(4);
}

std::vector<int> loadScoresFromJson() {
    std::vector<int> scores;
    json data;
    std::ifstream inFile("menu/score.json");
    if (inFile.is_open()) {
        inFile >> data;
        if (data.contains("scores") && data["scores"].is_array())
            for (auto &s : data["scores"])
                scores.push_back(s.get<int>());
    }
    std::sort(scores.begin(), scores.end(), std::greater<>());
    return scores;
}

bool isInside(const sf::Vector2i& point, const sf::RectangleShape& rect) {
    return rect.getGlobalBounds().contains(static_cast<sf::Vector2f>(point));
}

class Enemy {
public:
    enum class Type { CIRCLE, DOLPHIN };
    
    Enemy(Type type, const std::vector<sf::Vector2i>& path, float speed = 1.0f) 
        : type(type), path(path), speed(speed), currentWaypoint(0), health(100), maxHealth(100) {
        
        if (type == Type::DOLPHIN) {
            static bool texturesLoaded = false;
            static sf::Texture tex1, tex2;
            
            if (!texturesLoaded) {
                std::string tex1Path = getAssetPath("sprites/dolphin/dolphin_inferno_01.png");
                std::string tex2Path = getAssetPath("sprites/dolphin/dolphin_inferno_02.png");
                std::cout << "Loading dolphin textures from:\n" << tex1Path << "\n" << tex2Path << std::endl;
            
                if (!tex1.loadFromFile(tex1Path) || !tex2.loadFromFile(tex2Path)) {
                    std::cerr << "Failed to load dolphin textures!" << std::endl;
                    return;
                }
                texturesLoaded = true;
            }
            
            sprite.setTexture(tex1);
            sprite.setTextureRect(sf::IntRect(0, 0, tex1.getSize().x, tex1.getSize().y));
            sprite.setOrigin(tex1.getSize().x / 2, tex1.getSize().y / 2);
            sprite.setScale(0.12f, 0.12f);
            // Store texture pointers for animation
            texture1 = &tex1;
            texture2 = &tex2;
        } else {
            circle.setRadius(15);
            circle.setOrigin(15, 15);
            // Random color for circle enemies
            circle.setFillColor(sf::Color(
                rand() % 205 + 50,  // R: 50-255
                rand() % 205 + 50,  // G: 50-255
                rand() % 205 + 50   // B: 50-255
            ));
        }
        
        if (!path.empty()) {
            setPosition(GRID_START_X + path[0].x * TILE_SIZE + TILE_SIZE/2, 
                       GRID_START_Y + path[0].y * TILE_SIZE + TILE_SIZE/2);
        }
    }
    
    void update(float deltaTime) {
        if (currentWaypoint >= path.size() - 1) return;
        
        sf::Vector2f currentPos = getPosition();
        sf::Vector2f targetPos(
            GRID_START_X + path[currentWaypoint + 1].x * TILE_SIZE + TILE_SIZE/2,
            GRID_START_Y + path[currentWaypoint + 1].y * TILE_SIZE + TILE_SIZE/2
        );
        
        sf::Vector2f direction = targetPos - currentPos;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        
        if (distance < 5.0f) {
            currentWaypoint++;
        } else {
            direction /= distance;
            move(direction * speed * deltaTime * 60.0f);
            
            // Update dolphin animation
            if (type == Type::DOLPHIN) {
                animTimer += deltaTime;
                if (animTimer >= 0.15f) { // Slightly faster animation
                    animTimer = 0;
                    currentFrame = 1 - currentFrame;
                    sprite.setTexture(currentFrame ? *texture2 : *texture1);
                    sprite.setTextureRect(sf::IntRect(0, 0, texture1->getSize().x, texture1->getSize().y));
                }
                
                // Rotate dolphin to face direction
                if (distance > 0) {
                    float angle = std::atan2(direction.y, direction.x) * 180.0f / 3.14159f;
                    sprite.setRotation(angle);
                }
            }
        }
    }
    
    void render(sf::RenderWindow& window) {
        if (type == Type::DOLPHIN) {
            window.draw(sprite);
        } else {
            window.draw(circle);
        }
    }
    
    bool hasReachedEnd() const { return currentWaypoint >= path.size() - 1; }
    
    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
    }
    
    bool isAlive() const { return health > 0; }
    
    void setPosition(float x, float y) {
        if (type == Type::DOLPHIN) {
            sprite.setPosition(x, y);
        } else {
            circle.setPosition(x, y);
        }
    }
    
    sf::Vector2f getPosition() const {
        return type == Type::DOLPHIN ? sprite.getPosition() : circle.getPosition();
    }
    
    void move(const sf::Vector2f& offset) {
        if (type == Type::DOLPHIN) {
            sprite.move(offset);
        } else {
            circle.move(offset);
        }
    }
    
private:
    Type type;
    std::vector<sf::Vector2i> path;
    size_t currentWaypoint;
    float speed;
    int health;
    int maxHealth;
    
    // Dolphin specific
    const sf::Texture* texture1 = nullptr;
    const sf::Texture* texture2 = nullptr;
    sf::Sprite sprite;
    float animTimer = 0;
    int currentFrame = 0;
    
    // Circle specific
    sf::CircleShape circle;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Tower Defense");
    window.setFramerateLimit(60);
    window.setVerticalSyncEnabled(true);

    // Initialize models
    GameModel gameModel;
    ScoreModel scoreModel;

    // Initialize views
    GameView gameView;
    MenuView menuView;

    // Initialize controllers
    GameController gameController(gameModel, gameView);
    MenuController menuController(gameModel, menuView);

    // Background music
    sf::Music backgroundMusic;
    std::string musicPath = getAssetPath("music/background.mp3");
    std::cout << "Loading music from: " << musicPath << std::endl;
    if (backgroundMusic.openFromFile(musicPath)) {
        backgroundMusic.setLoop(true);
        backgroundMusic.setVolume(50);
        backgroundMusic.play();
    } else {
        std::cerr << "Failed to load background music from: " << musicPath << std::endl;
    }

    sf::Clock clock;

    // Debug: Print initial state
    std::cout << "Game starting. Initial state: " << static_cast<int>(gameModel.getState()) << std::endl;
    
    // Main game loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
                continue;
            }

            // Debug: Print event info
            if (event.type == sf::Event::MouseButtonPressed) {
                std::cout << "Mouse pressed at: (" << event.mouseButton.x 
                          << ", " << event.mouseButton.y << ")" << std::endl;
            }

            // Handle input based on game state
            GameModel::GameState currentState = gameModel.getState();
            if (currentState == GameModel::GameState::PLAYING) {
                gameController.handleInput(event, window);
            } else {
                menuController.handleInput(event, window);
            }
            
            // Check if state changed
            if (currentState != gameModel.getState()) {
                std::cout << "Game state changed to: " 
                          << static_cast<int>(gameModel.getState()) << std::endl;
            }
        }

        // Update game state
        float deltaTime = clock.restart().asSeconds();
        if (gameModel.getState() == GameModel::GameState::PLAYING) {
            gameModel.update(deltaTime);
        }

        // Render the appropriate view
        window.clear();
        
        switch (gameModel.getState()) {
            case GameModel::GameState::PLAYING:
                gameView.render(window, gameModel);
                break;
            case GameModel::GameState::MENU:
            case GameModel::GameState::SCORE:
                menuView.render(window, gameModel, scoreModel);
                break;
            default:
                std::cerr << "Unknown game state: " << static_cast<int>(gameModel.getState()) << std::endl;
                break;
        }

        window.display();
    }

    return 0;
}