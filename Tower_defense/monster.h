#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>

// Enum for monster types
enum class MonsterType {
    Red, Blue, Green, Yellow, Purple, Dolphin
};

// Path structure for monsters to follow
struct Path {
    std::vector<sf::Vector2f> waypoints;
    Path() = default;
    Path(const std::vector<sf::Vector2f>& pts) : waypoints(pts) {}
};

class Monster {
public:
    Monster(const Path* pathRef, MonsterType type = MonsterType::Red);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window) const;
    bool hasReachedEnd() const;
    const sf::Vector2f& getPosition() const;
    static void loadDolphinTextures();
    
    // Add missing methods
    bool isAlive() const { return health > 0; }
    void takeDamage(float damage) { health -= damage; }

private:
    const Path* path;
    float speed;
    int currentWaypoint;
    float health;
    MonsterType type;
    sf::CircleShape shape;
    sf::Sprite sprite;
    float dolphinAnimTimer = 0.0f;
    int dolphinAnimFrame = 0;
    static constexpr float dolphinAnimInterval = 0.15f;
    static sf::Texture dolphinTexture1;
    static sf::Texture dolphinTexture2;
    static bool dolphinTexturesLoaded;
};
