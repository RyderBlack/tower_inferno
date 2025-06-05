#ifndef TOWER_INFERNO_ENEMY_H
#define TOWER_INFERNO_ENEMY_H

#include <raylib.h>
#include <vector>

enum class EnemyType {
    BASIC,
    FAST,
    TANK,
    FLYING,
    BOSS
};

class Enemy {
private:
    EnemyType type;
    Vector2 position;
    Vector2 targetPosition;
    float speed;
    int health;
    int maxHealth;
    int damage;
    int goldValue;
    int pathIndex;
    std::vector<Vector2> path;
    bool isActive;
    
public:
    Enemy(EnemyType type, const std::vector<Vector2>& path);
    
    void update(float deltaTime);
    void takeDamage(int amount);
    void draw() const;
    
    // Getters
    bool getIsActive() const { return isActive; }
    Vector2 getPosition() const { return position; }
    int getHealth() const { return health; }
    int getMaxHealth() const { return maxHealth; }
    int getDamage() const { return damage; }
    int getGoldValue() const { return goldValue; }
    EnemyType getType() const { return type; }
    
private:
    void move(float deltaTime);
    void updatePathTarget();
    void initializeFromType();
    
    // Helper functions for movement
    bool hasReachedCurrentTarget() const;
    float calculateDistance(const Vector2& a, const Vector2& b) const;
};

#endif //TOWER_INFERNO_ENEMY_H
