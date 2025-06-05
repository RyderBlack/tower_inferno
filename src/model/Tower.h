#ifndef TOWER_INFERNO_TOWER_H
#define TOWER_INFERNO_TOWER_H

#include <raylib.h>
#include <memory>
#include "../utils/Observable.h"

// Forward declaration
class Enemy;

enum class TowerType {
    BASIC,
    SNIPER,
    SPLASH,
    SLOW,
    POISON
};

class Tower : public Observable {
private:
    TowerType type;
    Vector2 position;
    float range;
    float damage;
    float attackSpeed;
    float attackCooldown;
    int level;
    int upgradeCost;
    int sellValue;
    
    // Special effects
    float slowAmount;  // For slow towers
    float splashRadius; // For splash towers
    float poisonDamage; // For poison towers
    int poisonDuration; // For poison towers
    
    bool isActive;
    
public:
    Tower(TowerType type, const Vector2& position);
    
    void update(float deltaTime, const std::vector<std::unique_ptr<Enemy>>& enemies);
    void draw() const;
    void upgrade();
    void sell();
    
    // Getters
    TowerType getType() const { return type; }
    Vector2 getPosition() const { return position; }
    float getRange() const { return range; }
    int getLevel() const { return level; }
    int getUpgradeCost() const { return upgradeCost; }
    int getSellValue() const { return sellValue; }
    bool getIsActive() const { return isActive; }
    
private:
    void initializeFromType();
    void shoot(Enemy* target);
    Enemy* findTarget(const std::vector<std::unique_ptr<Enemy>>& enemies) const;
    bool isInRange(const Vector2& targetPos) const;
    
    // Helper functions for different tower types
    void applyBasicAttack(Enemy* target);
    void applySplashAttack(Enemy* target, const std::vector<std::unique_ptr<Enemy>>& enemies);
    void applySlowEffect(Enemy* target);
    void applyPoisonEffect(Enemy* target);
};

#endif //TOWER_INFERNO_TOWER_H
