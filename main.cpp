#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <cmath>

// Forward declarations
class Enemy;

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
class Enemy {
private:
    sf::CircleShape shape;
    const Path* path;
    float speed;
    int currentWaypoint;
    float health;
    
public:
    Enemy(const Path* pathRef, float size = 15.0f, float speed = 1.5f) 
        : path(pathRef), speed(speed), currentWaypoint(0), health(100.0f) {
        shape.setRadius(size);
        shape.setOrigin(size, size);
        shape.setFillColor(sf::Color::Red);
        
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
    const float spawnInterval = 2.0f; // Spawn a new enemy every 2 seconds
    
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

        // Spawn new enemies
        if (spawnClock.getElapsedTime().asSeconds() >= spawnInterval) {
            enemies.emplace_back(&path);
            spawnClock.restart();
        }

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

        // Display everything
        window.display();
    }

    std::cout << "Game ended" << std::endl;
    return 0;
}
