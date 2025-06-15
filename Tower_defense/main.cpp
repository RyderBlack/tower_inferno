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

using json = nlohmann::json;

// Game constants
const int WINDOW_WIDTH = 1600;  // Fixed window width
const int WINDOW_HEIGHT = 800;  // Fixed window height
const int UI_WIDTH = 300;       // Width of the UI panel
const int TILE_SIZE = 72;       // Increased tile size to 64x64 pixels
const int GRID_WIDTH = 18;      // Number of horizontal tiles (reduced to fit window with larger tiles)
const int GRID_HEIGHT = 11;     // Number of vertical tiles (reduced to fit window with larger tiles)
const int GRID_START_X = (WINDOW_WIDTH - UI_WIDTH - (GRID_WIDTH * TILE_SIZE)) / 2;
const int GRID_START_Y = (WINDOW_HEIGHT - (GRID_HEIGHT * TILE_SIZE)) / 2;

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
                if (!tex1.loadFromFile("assets/sprites/dolphin/dolphin_inferno_01.png") ||
                    !tex2.loadFromFile("assets/sprites/dolphin/dolphin_inferno_02.png")) {
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
    window.setSize(sf::Vector2u(WINDOW_WIDTH, WINDOW_HEIGHT));

    // Load menu assets
    sf::Texture backgroundTexture, playTexture, scoreTexture, quitTexture, volumeTexture, scoreBackgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/image/tower_inferno_cover.png") ||
        !playTexture.loadFromFile("assets/image/play_button.png") ||
        !scoreTexture.loadFromFile("assets/image/score_button.png") ||
        !quitTexture.loadFromFile("assets/image/quit_icon.png") ||
        !volumeTexture.loadFromFile("assets/image/volume_icon.png") ||
        !scoreBackgroundTexture.loadFromFile("assets/image/tower_inferno_fon.png")) {
        std::cerr << "Failed to load menu textures!" << std::endl;
        return -1;
    }

    // Menu sprites
    sf::Sprite backgroundSprite(backgroundTexture);
    sf::Sprite playButton(playTexture);
    sf::Sprite scoreButton(scoreTexture);
    sf::Sprite quitButton(quitTexture);
    sf::Sprite volumeButton(volumeTexture);
    sf::Sprite scoreBackground(scoreBackgroundTexture);

    // Scale menu buttons
    playButton.setScale(0.5f, 0.5f);
    volumeButton.setScale(0.2f, 0.2f);
    quitButton.setScale(0.2f, 0.2f);

    // Position menu elements
    float centerX = window.getSize().x / 2.f;
    float centerY = window.getSize().y / 2.f;

    playButton.setOrigin(playButton.getLocalBounds().width / 2, playButton.getLocalBounds().height / 2);
    playButton.setPosition(centerX, centerY + 90);

    scoreButton.setOrigin(scoreButton.getLocalBounds().width / 2, scoreButton.getLocalBounds().height / 2);
    scoreButton.setPosition(centerX, centerY + 290);

    volumeButton.setOrigin(volumeButton.getLocalBounds().width / 2, volumeButton.getLocalBounds().height / 2);
    volumeButton.setPosition(60, window.getSize().y - 60);

    quitButton.setOrigin(quitButton.getLocalBounds().width / 2, quitButton.getLocalBounds().height / 2);
    quitButton.setPosition(window.getSize().x - 60, window.getSize().y - 60);

    // Load font
    sf::Font font;
    if (!font.loadFromFile("assets/fonts/fast99.ttf")) {
        std::cerr << "Failed to load font!" << std::endl;
        return -1;
    }

    // Menu text
    sf::Text soundStateText("Sound: On", font, 20);
    soundStateText.setFillColor(sf::Color::White);
    soundStateText.setOrigin(soundStateText.getLocalBounds().width / 2, 0);
    soundStateText.setPosition(60, window.getSize().y - 30);

    sf::Text scoreTitle("SCORES", font, 48);
    scoreTitle.setStyle(sf::Text::Bold);
    scoreTitle.setFillColor(sf::Color::White);
    scoreTitle.setPosition(centerX - scoreTitle.getLocalBounds().width / 2, 50);

    sf::Text scoreBackButton("Back", font, 24);
    scoreBackButton.setFillColor(sf::Color::White);
    scoreBackButton.setPosition(50, 50);

    // Background music
    sf::Music backgroundMusic;
    if (backgroundMusic.openFromFile("assets/music/background.mp3")) {
        backgroundMusic.setLoop(true);
        backgroundMusic.setVolume(50);
        backgroundMusic.play();
    }

    bool soundEnabled = true;
    GameState state = GameState::MENU;

    // Load textures
    sf::Texture rockTexture, lava1Texture, lava2Texture, lava3Texture, lava4Texture, lava5Texture, lava6Texture, goldTexture, heartTexture;
    
    // Load each texture separately with error checking
    auto loadTexture = [](sf::Texture& texture, const std::string& path) -> bool {
        if (!texture.loadFromFile(path)) {
            std::cerr << "Failed to load texture: " << path << std::endl;
            return false;
        }
        return true;
    };

    // Try to load each texture and check for errors
    bool allTexturesLoaded = true;
    
    if (!loadTexture(rockTexture, "assets/textures/tower_inferno_ground_tile.png")) allTexturesLoaded = false;
    if (!loadTexture(lava1Texture, "assets/textures/tower_inferno_path_02.png")) allTexturesLoaded = false;  // horizontal path
    if (!loadTexture(lava2Texture, "assets/textures/tower_inferno_path_01.png")) allTexturesLoaded = false;  // vertical path
    if (!loadTexture(lava3Texture, "assets/textures/tower_inferno_corner_03.png")) allTexturesLoaded = false; // virage_gauche_bas
    if (!loadTexture(lava4Texture, "assets/textures/tower_inferno_corner_02.png")) allTexturesLoaded = false; // virage_haut_droite (right turn from top)
    if (!loadTexture(lava5Texture, "assets/textures/tower_inferno_corner_04.png")) allTexturesLoaded = false; // virage_gauche_haut (left turn from top)
    if (!loadTexture(lava6Texture, "assets/textures/tower_inferno_corner_01.png")) allTexturesLoaded = false; // virage_bas_droite
    if (!loadTexture(goldTexture, "assets/textures/Towers1.png")) allTexturesLoaded = false;
    if (!loadTexture(heartTexture, "assets/textures/heart.png")) allTexturesLoaded = false;
    
    if (!allTexturesLoaded) {
        std::cerr << "Failed to load one or more textures. Check the paths above." << std::endl;
        // Keep the window open for a few seconds to see the error message
        sf::sleep(sf::seconds(5));
        return -1;
    }

    // UI Setup
    sf::Text goldText;
    goldText.setFont(font);
    goldText.setCharacterSize(30);
    goldText.setFillColor(sf::Color::Yellow);
    goldText.setPosition(70.f, 25.f);
    sf::Sprite goldSprite(goldTexture);
    goldSprite.setPosition(20.f, 20.f);

    int lives = 3;
    int gold = 5000;
    
    // Note: TILE_SIZE, GRID_WIDTH, and GRID_HEIGHT are already defined at the top of the file
    // These are the main values to adjust if you want to change the grid size:
    // - TILE_SIZE: Size of each grid cell in pixels (currently 50)
    // - GRID_WIDTH: Number of tiles wide (currently 30)
    // - GRID_HEIGHT: Number of tiles high (currently 20)
    
    // Path from top-left to bottom-right with multiple turns
    std::vector<sf::Vector2i> path = {
        // Start at top-left
        {0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {5, 0}, {6, 0}, {7, 0}, {8, 0}, {9, 0}, {10, 0},
        // Down
        {10, 1}, {10, 2}, {10, 3}, {10, 4}, {10, 5}, {10, 6},
        // Left
        {9, 6}, {8, 6}, {7, 6}, {6, 6}, {5, 6},
        // Down to exit at bottom-right
        {5, 7}, {5, 8}, {5, 9}, {5, 10},
        // Right to exit
        {6, 10}, {7, 10}, {8, 10}, {9, 10}, {10, 10}, {11, 10}, {12, 10}
    };

    enum TileType {
        TILE_ROCK = 0,
        HORIZONTALE_LAVA = 1,
        VERTICALE_LAVA = 2,
        VIRAGE_GAUCHE_BAS = 3,
        VIRAGE_HAUT_DROITE = 4,
        VIRAGE_GAUCHE_HAUT = 5,
        VIRAGE_BAS_DROITE = 6
    };

    // Initialize grid with new dimensions
    std::vector<std::vector<int>> grid(GRID_HEIGHT, std::vector<int>(GRID_WIDTH, TILE_ROCK));

    // Mark path on grid
    for (size_t i = 0; i < path.size() - 1; ++i) {
        int x1 = path[i].x;
        int y1 = path[i].y;
        int x2 = path[i+1].x;
        int y2 = path[i+1].y;
        
        if (x1 == x2) { // Vertical path
            int startY = std::min(y1, y2);
            int endY = std::max(y1, y2);
            for (int y = startY; y <= endY; ++y) {
                if (y < 0 || y >= GRID_HEIGHT || x1 < 0 || x1 >= GRID_WIDTH) continue;
                
                // Check for corners
                bool isCorner = false;
                if (i > 0 && i < path.size() - 1) {
                    int px = path[i-1].x;
                    int py = path[i-1].y;
                    int nx = path[i+1].x;
                    int ny = path[i+1].y;
                    
                    // Only mark as corner if this is a 90-degree turn
                    bool isHorizontal = (px == x1 && x1 == nx); // Moving horizontally through this point
                    bool isVertical  = (py == y1 && y1 == ny);  // Moving vertically through this point
                    isCorner = !(isHorizontal || isVertical);   // If not straight, it's a corner
                    
                    if (isCorner) {
                        // Determine corner type based on direction
                        int px = path[i-1].x;
                        
                        if (px < x1) { // Coming from left
                                if (y > path[i].y) grid[y][x1] = VIRAGE_GAUCHE_HAUT; // Going down
                                else grid[y][x1] = VIRAGE_GAUCHE_BAS; // Going up
                            } else if (px > x1) { // Coming from right
                                if (y > path[i].y) grid[y][x1] = VIRAGE_HAUT_DROITE; // Going down
                                else grid[y][x1] = VIRAGE_BAS_DROITE; // Going up
                            } else if (py < y1) { // Coming from above
                                if (x1 > path[i].x) grid[y][x1] = VIRAGE_GAUCHE_HAUT; // Going right
                                else grid[y][x1] = VIRAGE_HAUT_DROITE; // Going left
                            } else { // Coming from below
                                if (x1 > path[i].x) grid[y][x1] = VIRAGE_GAUCHE_BAS; // Going right
                                else grid[y][x1] = VIRAGE_BAS_DROITE; // Going left
                            }
                    } else {
                        grid[y][x1] = VERTICALE_LAVA;
                    }
                } else {
                    grid[y][x1] = VERTICALE_LAVA;
                }
            }
        } else if (y1 == y2) { // Horizontal path
            int startX = std::min(x1, x2);
            int endX = std::max(x1, x2);
            for (int x = startX; x <= endX; ++x) {
                if (y1 < 0 || y1 >= GRID_HEIGHT || x < 0 || x >= GRID_WIDTH) continue;
                
                // Check for corners
                bool isCorner = false;
                if (i > 0 && i < path.size() - 1) {
                    int px = path[i-1].x;
                    int py = path[i-1].y;
                    int nx = path[i+1].x;
                    int ny = path[i+1].y;
                    
                    // Only mark as corner if this is a 90-degree turn
                    bool isHorizontal = (px == x && x == nx);   // Using x instead of x1 here
                    bool isVertical  = (py == y1 && y1 == ny);  // Moving vertically through this point
                    isCorner = !(isHorizontal || isVertical);   // If not straight, it's a corner
                    
                    if (isCorner) {
                        // Determine corner type based on direction
                        int py = path[i-1].y;
                        
                        if (py < y1) { // Coming from above
                                if (x > path[i].x) grid[y1][x] = VIRAGE_GAUCHE_HAUT; // Going right
                                else grid[y1][x] = VIRAGE_HAUT_DROITE; // Going left
                            } else { // Coming from below
                                if (x > path[i].x) grid[y1][x] = VIRAGE_GAUCHE_BAS; // Going right
                                else grid[y1][x] = VIRAGE_BAS_DROITE; // Going left
                            }
                    } else {
                        grid[y1][x] = HORIZONTALE_LAVA;
                    }
                } else {
                    grid[y1][x] = HORIZONTALE_LAVA;
                }
            }
        }
    }

    // Game objects
    std::vector<std::unique_ptr<Tower>> towers;
    std::vector<Enemy> enemies;
    TowerSelectionUI towerUI(&font);

    // UI Elements
    sf::RectangleShape upgradeButton({100.f, 30.f});
    sf::RectangleShape sellButton({100.f, 30.f});
    upgradeButton.setFillColor(sf::Color(100, 100, 255));
    sellButton.setFillColor(sf::Color(255, 100, 100));

    sf::Text upgradeText("Upgrade", font, 20);
    sf::Text sellText("Vendre", font, 20);
    upgradeText.setFillColor(sf::Color::White);
    sellText.setFillColor(sf::Color::White);

    // Game state
    bool showTowerButtons = false;
    bool menuJustOpened = false;
    int selectedTowerIndex = -1;
    sf::Vector2f buttonPosition;
    sf::Vector2i selectedGridPos;
    
    // Enemy spawning
    sf::Clock spawnClock;
    float spawnInterval = 2.0f; // seconds
    int wave = 1;
    int enemiesInWave = 5;
    int enemiesSpawned = 0;
    bool waveInProgress = false;
    
    // Game loop
    sf::Clock clock;
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mousePos(sf::Mouse::getPosition(window));

                if (state == GameState::MENU) {
                    if (playButton.getGlobalBounds().contains(mousePos)) {
                        state = GameState::PLAYING;
                    } else if (scoreButton.getGlobalBounds().contains(mousePos)) {
                        state = GameState::SCORE;
                    } else if (volumeButton.getGlobalBounds().contains(mousePos)) {
                        soundEnabled = !soundEnabled;
                        soundStateText.setString(soundEnabled ? "Sound: On" : "Sound: Off");
                        soundStateText.setOrigin(soundStateText.getLocalBounds().width / 2, 0);
                        soundEnabled ? backgroundMusic.play() : backgroundMusic.pause();
                    } else if (quitButton.getGlobalBounds().contains(mousePos)) {
                        window.close();
                    }
                } else if (state == GameState::SCORE) {
                    if (scoreBackButton.getGlobalBounds().contains(mousePos)) {
                        state = GameState::MENU;
                    }
                } else if (state == GameState::PLAYING) {
                    // Calculate grid position based on mouse position, accounting for grid start position and tile size
                    sf::Vector2i gridPos(
                        (static_cast<int>(mousePos.x) - GRID_START_X) / TILE_SIZE,
                        (static_cast<int>(mousePos.y) - GRID_START_Y) / TILE_SIZE
                    );
                    
                    // Ensure grid position is within bounds
                    gridPos.x = std::max(0, std::min(GRID_WIDTH - 1, gridPos.x));
                    gridPos.y = std::max(0, std::min(GRID_HEIGHT - 1, gridPos.y));
                    
                    // Debug output to help diagnose placement issues
                    std::cout << "Mouse: (" << mousePos.x << ", " << mousePos.y << ") "
                              << "Grid: (" << gridPos.x << ", " << gridPos.y << ") "
                              << "GRID_START: (" << GRID_START_X << ", " << GRID_START_Y << ") "
                              << "TILE_SIZE: " << TILE_SIZE << std::endl;

                    if (towerUI.isVisible()) {
                        TowerType selectedType = towerUI.checkClick(sf::Vector2i(static_cast<int>(mousePos.x), static_cast<int>(mousePos.y)));

                        if (selectedGridPos.x >= 0 && selectedGridPos.x < GRID_WIDTH &&
                            selectedGridPos.y >= 0 && selectedGridPos.y < GRID_HEIGHT &&
                            grid[selectedGridPos.y][selectedGridPos.x] == TILE_ROCK &&
                            gold >= TowerFactory::getTowerCost(selectedType)) {

                            bool alreadyPlaced = false;
                            for (auto& tower : towers) {
                                sf::Vector2i towerGridPos(
                                    static_cast<int>(std::round((tower->position.x - GRID_START_X) / static_cast<float>(TILE_SIZE))),
                                    static_cast<int>(std::round((tower->position.y - GRID_START_Y) / static_cast<float>(TILE_SIZE)))
                                );
                                if (towerGridPos == selectedGridPos) {
                                    alreadyPlaced = true;
                                    break;
                                }
                            }

                            if (!alreadyPlaced) {
                                auto newTower = TowerFactory::createTower(
                                    selectedType, 
                                    selectedGridPos
                                );
                                
                                newTower->gridPosition = selectedGridPos;
                                if (newTower) {
                                    gold -= newTower->cost;
                                    towers.push_back(std::move(newTower));
                                }
                            }
                        }
                        towerUI.hide();
                        continue;
                    }

                    if (showTowerButtons && !menuJustOpened) {
                        if (isInside(sf::Vector2i(mousePos), upgradeButton)) {
                            if (selectedTowerIndex >= 0 &&
                                towers[selectedTowerIndex]->level < towers[selectedTowerIndex]->maxLevel &&
                                gold >= towers[selectedTowerIndex]->upgradeCost) {
                                gold -= towers[selectedTowerIndex]->upgradeCost;
                                towers[selectedTowerIndex]->upgrade();
                            }
                            showTowerButtons = false;
                            selectedTowerIndex = -1;
                            continue;
                        } else if (isInside(sf::Vector2i(mousePos), sellButton)) {
                            if (selectedTowerIndex >= 0) {
                                gold += towers[selectedTowerIndex]->getSellPrice();
                                towers.erase(towers.begin() + selectedTowerIndex);
                            }
                            showTowerButtons = false;
                            selectedTowerIndex = -1;
                            continue;
                        }
                    }

                    bool clickedOnTower = false;
                    for (size_t i = 0; i < towers.size(); ++i) {
                        if (towers[i]->position == gridPos) {
                            selectedTowerIndex = static_cast<int>(i);
                            buttonPosition = mousePos;
                            upgradeButton.setPosition(buttonPosition);
                            sellButton.setPosition(buttonPosition.x, buttonPosition.y + 35);
                            upgradeText.setPosition(buttonPosition.x + 10, buttonPosition.y + 2);
                            sellText.setPosition(buttonPosition.x + 10, buttonPosition.y + 37);
                            showTowerButtons = true;
                            menuJustOpened = true;
                            clickedOnTower = true;
                            break;
                        }
                    }

                    if (!clickedOnTower && gridPos.x >= 0 && gridPos.x < GRID_WIDTH &&
                        gridPos.y >= 0 && gridPos.y < GRID_HEIGHT && grid[gridPos.y][gridPos.x] == TILE_ROCK) {
                        selectedGridPos = gridPos;
                        towerUI.show(mousePos);
                        showTowerButtons = false;
                        selectedTowerIndex = -1;
                    } else if (!clickedOnTower) {
                        showTowerButtons = false;
                        selectedTowerIndex = -1;
                    }
                }
            }

            if (state == GameState::PLAYING && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                saveScoreToJson(gold);
                state = GameState::MENU;
            }
        }

        window.clear();

        if (state == GameState::MENU) {
            window.draw(backgroundSprite);
            window.draw(playButton);
            window.draw(scoreButton);
            window.draw(quitButton);
            window.draw(volumeButton);
            window.draw(soundStateText);
        } else if (state == GameState::SCORE) {
            window.draw(scoreBackground);
            window.draw(scoreTitle);
            window.draw(scoreBackButton);

            // Display scores
            auto scores = loadScoresFromJson();
            for (size_t i = 0; i < std::min(scores.size(), size_t(10)); ++i) {
                sf::Text scoreText(std::to_string(i + 1) + ". " + std::to_string(scores[i]), font, 24);
                scoreText.setFillColor(sf::Color::White);
                scoreText.setPosition(centerX - scoreText.getLocalBounds().width / 2, 150 + i * 40);
                window.draw(scoreText);
            }
        } else if (state == GameState::PLAYING) {
            // Start new wave if needed
            if (!waveInProgress && enemies.empty()) {
                waveInProgress = true;
                enemiesSpawned = 0;
                spawnClock.restart();
                wave++;
                enemiesInWave = 5 + wave * 2; // Increase enemies per wave
            }

            // Spawn enemies
            if (waveInProgress && enemiesSpawned < enemiesInWave && spawnClock.getElapsedTime().asSeconds() >= spawnInterval) {
                // Alternate between dolphin and circle enemies
                Enemy::Type type = (enemiesSpawned % 3 == 0) ? Enemy::Type::DOLPHIN : Enemy::Type::CIRCLE;
                float speed = (type == Enemy::Type::DOLPHIN) ? 1.5f : 1.0f; // Dolphins are faster
                
                enemies.emplace_back(type, path, speed);
                enemiesSpawned++;
                spawnClock.restart();
                
                if (enemiesSpawned >= enemiesInWave) {
                    waveInProgress = false;
                }
            }

            // Update game state
            for (auto it = enemies.begin(); it != enemies.end(); ) {
                it->update(deltaTime);
                
                // Check if enemy reached the end
                if (it->hasReachedEnd()) {
                    lives--;
                    it = enemies.erase(it);
                    if (lives <= 0) {
                        saveScoreToJson(gold);
                        state = GameState::MENU;
                    }
                } else {
                    ++it;
                }
            }

            // Update towers and check for targets
            for (auto& tower : towers) {
                // Find target
                float closestDist = tower->stats.range * TILE_SIZE;
                Enemy* target = nullptr;
                
                for (auto& enemy : enemies) {
                    float dist = std::sqrt(
                        std::pow(enemy.getPosition().x - tower->position.x, 2) +
                        std::pow(enemy.getPosition().y - tower->position.y, 2)
                    );
                    
                    if (dist < closestDist) {
                        closestDist = dist;
                        target = &enemy;
                    }
                }
                
                // Attack target if in range and cooldown is ready
                if (target && tower->canAttack(tower->lastAttackTime)) {
                    target->takeDamage(tower->stats.damage);
                    tower->lastAttackTime = 0;
                    
                    // Apply effects
                    if (tower->stats.effect != EffectType::NONE) {
                        // Handle effects (you can expand this)
                    }
                    
                    // Remove dead enemies and award gold
                    if (!target->isAlive()) {
                        gold += 10; // Gold for killing an enemy
                        enemies.erase(std::remove_if(enemies.begin(), enemies.end(), 
                            [](const Enemy& e) { return !e.isAlive(); }), enemies.end());
                    }
                }
                
                tower->lastAttackTime += deltaTime;
            }

            // Draw the grid
            for (int y = 0; y < GRID_HEIGHT; ++y) {
                for (int x = 0; x < GRID_WIDTH; ++x) {
                    sf::Sprite tile;
                    sf::Texture* currentTexture = nullptr;
                    
                    // Set the appropriate texture
                    switch (grid[y][x]) {
                        case TILE_ROCK: currentTexture = &rockTexture; break;
                        case HORIZONTALE_LAVA: currentTexture = &lava1Texture; break;
                        case VERTICALE_LAVA: currentTexture = &lava2Texture; break;
                        case VIRAGE_GAUCHE_BAS: currentTexture = &lava3Texture; break;
                        case VIRAGE_HAUT_DROITE: currentTexture = &lava4Texture; break;
                        case VIRAGE_GAUCHE_HAUT: currentTexture = &lava5Texture; break;
                        case VIRAGE_BAS_DROITE: currentTexture = &lava6Texture; break;
                    }
                    
                    if (currentTexture) {
                        tile.setTexture(*currentTexture);
                        tile.setTextureRect(sf::IntRect(0, 0, 1080, 1080));
                        float scale = static_cast<float>(TILE_SIZE) / 1080.0f;
                        tile.setScale(scale, scale);
                        tile.setPosition(GRID_START_X + x * TILE_SIZE, GRID_START_Y + y * TILE_SIZE);
                        window.draw(tile);
                    }
                }
            }

            // Draw towers
            for (auto& tower : towers) {
                tower->render(window, TILE_SIZE);
            }

            // Draw enemies
            for (auto& enemy : enemies) {
                enemy.render(window);
            }

            // Draw UI
            goldText.setString(std::to_string(gold));
            window.draw(goldSprite);
            window.draw(goldText);

            // Draw lives
            for (int i = 0; i < lives; ++i) {
                sf::Sprite heart(heartTexture);
                heart.setPosition(window.getSize().x - (i + 1) * (heart.getGlobalBounds().width + 10), 10.f);
                window.draw(heart);
            }

            // Draw wave info
            sf::Text waveText("Wave: " + std::to_string(wave), font, 30);
            waveText.setPosition(window.getSize().x - 150, 50);
            window.draw(waveText);

            // Draw tower UI if active
            if (showTowerButtons) {
                window.draw(upgradeButton);
                window.draw(sellButton);
                window.draw(upgradeText);
                window.draw(sellText);
            }

            // Draw tower selection UI
            towerUI.render(window);
        }

        window.display();
    }

    return 0;
}