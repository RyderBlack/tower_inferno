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
    COUNT
};

class Enemy {
private:
    sf::CircleShape shape;
    const Path* path;
    float speed;
    int currentWaypoint;
    float health;
    EnemyType type;
    
public:
    Enemy(const Path* pathRef, EnemyType enemyType = EnemyType::Red) 
        : path(pathRef), currentWaypoint(0), type(enemyType) {
        // Set properties based on enemy type
        switch (type) {
            case EnemyType::Blue:
                speed = 1.2f;  // Slowest
                shape.setRadius(12.0f);
                shape.setFillColor(sf::Color(0, 100, 255));  // Deep Blue
                break;
            case EnemyType::Green:
                speed = 1.6f;
                shape.setRadius(14.0f);
                shape.setFillColor(sf::Color(0, 200, 0));  // Green
                break;
            case EnemyType::Yellow:
                speed = 2.0f;
                shape.setRadius(16.0f);
                shape.setFillColor(sf::Color(255, 255, 0));  // Yellow
                break;
            case EnemyType::Purple:
                speed = 2.4f;
                shape.setRadius(18.0f);
                shape.setFillColor(sf::Color(160, 32, 240));  // Purple
                break;
            case EnemyType::Red:
            default:
                speed = 2.8f;  // Fastest
                shape.setRadius(20.0f);
                shape.setFillColor(sf::Color(255, 50, 50));  // Bright Red
                break;
        }
        
        // All enemies have the same health for simplicity
        health = 100.0f;
        
        shape.setOrigin(shape.getRadius(), shape.getRadius());
        
        // Start at the first waypoint
        if (path && !path->waypoints.empty()) {
            shape.setPosition(path->waypoints[0]);
        }
    }
    
    void update() {
        if (!path || currentWaypoint >= path->waypoints.size() - 1) {
            // Reached the end of the path
            return;
        }
        
        // Get current and target waypoints
        sf::Vector2f currentPos = shape.getPosition();
        sf::Vector2f targetPos = path->waypoints[currentWaypoint + 1];
        
        // Calculate direction vector
        sf::Vector2f direction = targetPos - currentPos;
        float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        
        // Normalize direction
        if (distance > 0) {
            direction /= distance;
        }
        
        // Move towards the target
        if (distance < 2.0f) {
            // Close enough to the waypoint, move to next one
            currentWaypoint++;
        } else {
            // Move towards the waypoint
            shape.move(direction * speed);
        }
    }
    
    void draw(sf::RenderWindow& window) const {
        window.draw(shape);
    }
    
    bool hasReachedEnd() const {
        return !path || currentWaypoint >= path->waypoints.size() - 1;
    }
    
    const sf::Vector2f& getPosition() const {
        return shape.getPosition();
    }
};

int main() {
    // Create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Tower Defense - Path Following");
    window.setFramerateLimit(60);

    std::cout << "Game started" << std::endl;

    // Create the path
    Path path;
    
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
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font. Using default font." << std::endl;
    }
    sf::Text waveText;
    waveText.setFont(font);
    waveText.setCharacterSize(24);
    waveText.setFillColor(sf::Color::White);
    waveText.setPosition(10, 10);
    
    // Game loop
    while (window.isOpen()) {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                std::cout << "Closing window" << std::endl;
                window.close();
            }
        }

        // Wave and enemy spawning logic
        float deltaTime = spawnClock.restart().asSeconds();
        
        if (!waveInProgress && enemies.empty()) {
            // Start new wave
            currentWave++;
            enemiesInWave = 5 + currentWave * 2;
            enemiesSpawned = 0;
            waveInProgress = true;
            spawnInterval = baseSpawnInterval * (1.0f - std::min(0.5f, currentWave * 0.05f)); // Speed up spawns as waves progress
            std::cout << "Starting wave " << currentWave << std::endl;
        }
        
        if (waveInProgress) {
            spawnTimer += deltaTime;
            
            // Spawn new enemies
            if (spawnTimer >= spawnInterval && enemiesSpawned < enemiesInWave) {
                // Cycle through enemy types in order
                static int typeIndex = 0;
                EnemyType type = static_cast<EnemyType>(typeIndex % 5);
                typeIndex = (typeIndex + 1) % 5;
                
                // Spawn 1-3 enemies at once in later waves
                int spawnCount = 1;
                if (currentWave >= 5 && getRandomInt(1, 5) == 1) {
                    spawnCount = getRandomInt(2, 3);
                }
                
                for (int i = 0; i < spawnCount && enemiesSpawned < enemiesInWave; i++) {
                    enemies.emplace_back(&path, type);
                    enemiesSpawned++;
                }
                
                spawnTimer = 0.0f;
            }
            
            // Check if wave is complete
            if (enemiesSpawned >= enemiesInWave && enemies.empty()) {
                waveInProgress = false;
                std::cout << "Wave " << currentWave << " complete!" << std::endl;
            }
        }
        
        // Update wave text
        waveText.setString("Wave: " + std::to_string(currentWave) + 
                          "\nEnemies: " + std::to_string(enemies.size()) + 
                          " / " + std::to_string(enemiesInWave));

        // Update game state
        for (auto& enemy : enemies) {
            enemy.update();
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
        window.draw(waveText);

        // Display everything
        window.display();
    }

    std::cout << "Game ended" << std::endl;
    return 0;
}
