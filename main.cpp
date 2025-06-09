#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <ctime>

// Forward declarations
class Enemy;

// Random number generator
int getRandomInt(int min, int max) {
    static std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
    std::uniform_int_distribution<int> dist(min, max);
    return dist(rng);
}

float getRandomFloat(float min, float max) {
    static std::mt19937 rng(static_cast<unsigned>(std::time(nullptr)));
    std::uniform_real_distribution<float> dist(min, max);
    return dist(rng);
}

// Path class to define the path enemies will follow
class Path {
public:
    std::vector<sf::Vector2f> waypoints;
    
    Path() {
        // Define a simple path (you can modify these points as needed)
        waypoints.push_back(sf::Vector2f(-50, 300));  // Start off-screen left
        waypoints.push_back(sf::Vector2f(200, 300));
        waypoints.push_back(sf::Vector2f(200, 100));
        waypoints.push_back(sf::Vector2f(600, 100));
        waypoints.push_back(sf::Vector2f(600, 500));
        waypoints.push_back(sf::Vector2f(850, 500));  // End off-screen right
    }
    
    void draw(sf::RenderWindow& window) {
        // Draw the path for visualization
        sf::VertexArray lines(sf::LineStrip, waypoints.size());
        for (size_t i = 0; i < waypoints.size(); ++i) {
            lines[i].position = waypoints[i];
            lines[i].color = sf::Color(100, 100, 100, 100);  // Semi-transparent gray
        }
        window.draw(lines);
    }
};

// Enemy class
// Enemy types
enum class EnemyType {
    Red,
    Blue,
    Green,
    Yellow,
    Purple,
    Dolphin,
    COUNT
};

class Enemy {
private:
    // Common properties
    const Path* path;
    float speed;
    int currentWaypoint;
    float health;
    EnemyType type;

    // Type-specific rendering
    sf::CircleShape shape; // For original enemies
    sf::Sprite sprite;     // For dolphin

public:
    // Static textures for Dolphin
    static sf::Texture dolphinTexture1;
    static sf::Texture dolphinTexture2;
    static bool dolphinTexturesLoaded;

    static void loadDolphinTextures() {
        if (!dolphinTexturesLoaded) {
            if (!dolphinTexture1.loadFromFile("C:/Users/ryrym_i6sf5hg/CLionProjects/tower_inferno/assets/sprites/dolphin/dolphin_inferno_01.png")) {
                std::cerr << "Failed to load dolphin_inferno_01.png" << std::endl;
            }
            // Only load/check texture1 for simplified static sprite
            if (dolphinTexture1.getSize().x > 0) {
                 dolphinTexturesLoaded = true;
                 // No need to load dolphinTexture2 for now
            } else {
                 dolphinTexturesLoaded = false;
                 std::cerr << "Dolphin texture dolphin_inferno_01.png failed to load properly." << std::endl;
            }
        }
    }

    Enemy(const Path* pathRef, EnemyType enemyType = EnemyType::Red) 
        : path(pathRef), currentWaypoint(0), type(enemyType) {
        
        health = 100.0f; // Common health
        const float speedMultiplier = 60.0f; // To convert old speed-per-frame to speed-per-second

        switch (type) {
            case EnemyType::Dolphin:
                speed = 1.8f * speedMultiplier;
                if (dolphinTexturesLoaded) {
                    sprite.setTexture(dolphinTexture1); // Always use frame 1
                    sprite.setOrigin(dolphinTexture1.getSize().x / 2.0f, dolphinTexture1.getSize().y / 2.0f);
                    sprite.setScale(0.1f, 0.1f); // Scale down to 10%
                } else {
                    shape.setRadius(15.f);
                    shape.setFillColor(sf::Color::Cyan); 
                    shape.setOrigin(15.f, 15.f);
                    std::cerr << "Dolphin instance created, but textures are not loaded. Using fallback shape." << std::endl;
                }
                break;
            case EnemyType::Blue:
                speed = 1.2f * speedMultiplier;
                shape.setRadius(12.0f);
                shape.setFillColor(sf::Color(0, 100, 255));
                shape.setOrigin(shape.getRadius(), shape.getRadius());
                break;
            case EnemyType::Green:
                speed = 1.6f * speedMultiplier;
                shape.setRadius(14.0f);
                shape.setFillColor(sf::Color(0, 200, 0));
                shape.setOrigin(shape.getRadius(), shape.getRadius());
                break;
            case EnemyType::Yellow:
                speed = 2.0f * speedMultiplier;
                shape.setRadius(16.0f);
                shape.setFillColor(sf::Color(255, 255, 0));
                shape.setOrigin(shape.getRadius(), shape.getRadius());
                break;
            case EnemyType::Purple:
                speed = 2.4f * speedMultiplier;
                shape.setRadius(18.0f);
                shape.setFillColor(sf::Color(160, 32, 240));
                shape.setOrigin(shape.getRadius(), shape.getRadius());
                break;
            case EnemyType::Red:
            default:
                speed = 2.8f * speedMultiplier;
                shape.setRadius(20.0f);
                shape.setFillColor(sf::Color(255, 50, 50));
                shape.setOrigin(shape.getRadius(), shape.getRadius());
                break;
        }
        
        // Start at the first waypoint
        if (path && !path->waypoints.empty()) {
            if (type == EnemyType::Dolphin && dolphinTexturesLoaded) {
                sprite.setPosition(path->waypoints[0]);
            } else {
                shape.setPosition(path->waypoints[0]);
            }
        }
    }
    
    void update(float deltaTime) { 
        if (!path || currentWaypoint >= path->waypoints.size() - 1) {
            return;
        }
        
        sf::Vector2f currentPos = getPosition();
        sf::Vector2f targetPos = path->waypoints[currentWaypoint + 1];
        
        sf::Vector2f direction = targetPos - currentPos;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        
        if (distance > 0) {
            direction /= distance;
        }
        
        // Simplified waypoint switching condition, similar to original fixed threshold
        // Threshold needs to be small enough to be accurate, but large enough to not get stuck.
        // Movement per frame is (speed * deltaTime). Let's use a threshold slightly larger than typical movement per frame.
        // e.g. if speed is 120 (2*60) and deltaTime is 1/60, movement is 2 pixels. Threshold of 5.0f should be fine.
        if (distance < 5.0f) { 
            currentWaypoint++;
            // No snap logic for now to simplify
        } else {
            // Move towards the waypoint using deltaTime
            if (type == EnemyType::Dolphin && dolphinTexturesLoaded) {
                sprite.move(direction * speed * deltaTime);
            } else {
                shape.move(direction * speed * deltaTime);
            }
        }
    }
    
    void draw(sf::RenderWindow& window) const {
        if (type == EnemyType::Dolphin && dolphinTexturesLoaded) {
            window.draw(sprite);
        } else {
            window.draw(shape); // Draws other types, or dolphin fallback
        }
    }
    
    bool hasReachedEnd() const {
        return !path || currentWaypoint >= path->waypoints.size() - 1;
    }
    
    const sf::Vector2f& getPosition() const {
        if (type == EnemyType::Dolphin && dolphinTexturesLoaded) {
            return sprite.getPosition();
        }
        return shape.getPosition(); // Fallback for Dolphin or other types
    }

    // Getter for debugging
    int getCurrentWaypointIndex() const {
        return currentWaypoint;
    }
};

// Static member definitions
sf::Texture Enemy::dolphinTexture1;
sf::Texture Enemy::dolphinTexture2; // Still defined but not used by loadDolphinTextures or animation logic now
bool Enemy::dolphinTexturesLoaded = false;

int main() {
    // Create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tower Defense - Path Following");
    window.setFramerateLimit(60);

    Enemy::loadDolphinTextures(); // Load dolphin textures once at the start
    if (Enemy::dolphinTexturesLoaded) {
        std::cout << "Dolphin textures loaded successfully." << std::endl << std::flush;
    } else {
        std::cout << "Dolphin textures FAILED to load. Dolphins will use fallback shapes." << std::endl << std::flush;
    }

    // Path object needs to be created before it's used by enemies or logging
    Path path;

    sf::Font font;
    sf::Text waveText; // Declare waveText earlier
    bool fontSuccessfullyLoaded = false; // Renamed from fontLoaded for clarity here, will map to existing 'fontLoaded'

    if (font.loadFromFile("arial.ttf")) {
        std::cout << "Font arial.ttf loaded successfully." << std::endl << std::flush;
        waveText.setFont(font); // Moved inside success block
        waveText.setCharacterSize(24);
        waveText.setFillColor(sf::Color::White);
        waveText.setPosition(10, 10);
        fontSuccessfullyLoaded = true;
    }
    else {
        std::cout << "[IMPORTANT] Failed to load font arial.ttf. Wave text will not be shown." << std::endl << std::flush;
        // fontSuccessfullyLoaded remains false, waveText is not configured with a bad font
    }

    std::cout << "Game started (after texture/font load attempts)" << std::endl << std::flush;

    // Create enemies
    std::vector<Enemy> enemies;
    enemies.reserve(100);  // Pre-allocate some space to avoid reallocations
    sf::Clock spawnClock;
    float spawnTimer = 0.0f;
    const float baseSpawnInterval = 0.8f; // Slightly faster base spawn interval
    float spawnInterval = baseSpawnInterval;
    
    // Wave system
    int currentWave = 0;
    int enemiesInWave = 5;
    int enemiesSpawned = 0;
    bool waveInProgress = false;
    // sf::Text waveText was already declared, fontSuccessfullyLoaded replaces the old fontLoaded for the draw guard
    
    // Game loop
    while (window.isOpen()) {
        float deltaTime = spawnClock.restart().asSeconds();

        // --- Start Diagnostic Log (MOVED TO TOP) ---
        // std::cout << "Loop Start | DeltaTime: " << deltaTime << " | Enemies: " << enemies.size() << std::flush;
        // if (!enemies.empty()) {
        //     const auto& firstEnemy = enemies[0];
        //     sf::Vector2f pos = firstEnemy.getPosition();
        //     int wp_idx = firstEnemy.getCurrentWaypointIndex();
        //     std::cout << " | FirstEnemy Pos: (" << pos.x << "," << pos.y << ") WP_Idx: " << wp_idx << std::flush;
        //     if (wp_idx < path.waypoints.size() -1 && wp_idx >=0) {
        //          sf::Vector2f targetWpPos = path.waypoints[wp_idx+1];
        //          std::cout << " TargetWP_Pos: (" << targetWpPos.x << "," << targetWpPos.y << ")" << std::flush;
        //     }
        // }
        // std::cout << std::endl << std::flush;
        // --- End Diagnostic Log ---

        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            std::cout << "  Event Polled: Type = " << event.type << std::flush;
            if (event.type == sf::Event::KeyPressed) {
                std::cout << " Key: " << event.key.code << std::flush;
            }
            std::cout << std::endl << std::flush;

            if (event.type == sf::Event::Closed) {
                std::cout << "  sf::Event::Closed received. Calling window.close()." << std::endl << std::flush;
                window.close();
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                std::cout << "  Escape key pressed. Calling window.close()." << std::endl << std::flush;
                window.close();
            }
        }

        // If window was closed by an event, we might want to skip the rest of the frame processing
        if (!window.isOpen()) {
            std::cout << "Window closed in event loop, skipping rest of frame." << std::endl << std::flush;
            continue; // Skip to next iteration (which will terminate the loop)
        }

        // Wave and enemy spawning logic
        if (!waveInProgress && enemies.empty()) {
            // Start new wave
            currentWave++;
            waveInProgress = true;
            spawnInterval = baseSpawnInterval * (1.0f - std::min(0.5f, currentWave * 0.05f));
            std::cout << "Starting wave " << currentWave << std::endl << std::flush;

            // Spawn a single dolphin at the start of the wave
            enemies.emplace_back(&path, EnemyType::Dolphin);
            // Spawn one of each colored circle at the start of the wave
            enemies.emplace_back(&path, EnemyType::Red);
            enemies.emplace_back(&path, EnemyType::Blue);
            enemies.emplace_back(&path, EnemyType::Green);
            enemies.emplace_back(&path, EnemyType::Yellow);
            enemies.emplace_back(&path, EnemyType::Purple);
        }
        
        // Update wave text
        waveText.setString("Wave: " + std::to_string(currentWave) + 
                          "\nEnemies: " + std::to_string(enemies.size()) + 
                          " / " + std::to_string(enemiesInWave));

        // Update game state
        for (auto& enemy : enemies) {
            enemy.update(deltaTime);
        }
        
        // Remove enemies that reached the end
        enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(),
                [](const Enemy& enemy) { return enemy.hasReachedEnd(); }),
            enemies.end()
        );

        // Drawing
        window.clear(sf::Color(50, 50, 50));  // Dark gray background
        
        // Draw the path
        path.draw(window);
        
        // Draw all enemies
        for (const auto& enemy : enemies) {
            enemy.draw(window);
        }
        
        // Draw UI
        if (fontSuccessfullyLoaded) { // Guard drawing with the new flag
            window.draw(waveText);
        }

        // Display everything
        window.display();
    }

    std::cout << "Game ended" << std::endl << std::flush;
    return 0;
}
