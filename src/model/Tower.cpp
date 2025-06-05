#include "Tower.h"

Tower::Tower(TowerType type, Vector2 position) 
    : type(type), position(position), level(1) {
    // Initialize tower properties based on type
    switch (type) {
        case TowerType::BASIC:
            range = 150.0f;
            damage = 20.0f;
            fireRate = 1.0f;
            cost = 100;
            upgradeCost = 50;
            break;
        case TowerType::SNIPER:
            range = 300.0f;
            damage = 100.0f;
            fireRate = 0.5f;
            cost = 200;
            upgradeCost = 100;
            break;
        case TowerType::SPLASH:
            range = 120.0f;
            damage = 40.0f;
            fireRate = 1.5f;
            splashRadius = 80.0f;
            cost = 150;
            upgradeCost = 75;
            break;
        case TowerType::SLOW:
            range = 130.0f;
            damage = 10.0f;
            fireRate = 0.8f;
            slowFactor = 0.5f;
            slowDuration = 2.0f;
            cost = 120;
            upgradeCost = 60;
            break;
        case TowerType::POISON:
            range = 140.0f;
            damage = 5.0f;
            fireRate = 2.0f;
            poisonDamage = 10.0f;
            poisonDuration = 3.0f;
            cost = 180;
            upgradeCost = 90;
            break;
    }
    
    timeSinceLastShot = 1.0f / fireRate; // Ready to shoot immediately
}

void Tower::update(float deltaTime, std::vector<std::shared_ptr<Enemy>>& enemies) {
    if (enemies.empty()) return;
    
    timeSinceLastShot += deltaTime;
    if (timeSinceLastShot < (1.0f / fireRate)) return;
    
    // Find target
    auto target = findTarget(enemies);
    if (target) {
        // Shoot at target
        timeSinceLastShot = 0.0f;
        onShoot(target);
    }
}

void Tower::upgrade() {
    level++;
    damage *= 1.5f;
    range *= 1.2f;
    fireRate *= 1.2f;
    upgradeCost = static_cast<int>(upgradeCost * 1.5f);
    
    // Special upgrades based on tower type
    switch (type) {
        case TowerType::SPLASH:
            splashRadius *= 1.2f;
            break;
        case TowerType::SLOW:
            slowFactor = std::max(0.1f, slowFactor * 0.9f); // Better slow
            slowDuration *= 1.2f;
            break;
        case TowerType::POISON:
            poisonDamage *= 1.3f;
            poisonDuration *= 1.2f;
            break;
        default:
            break;
    }
}

std::shared_ptr<Enemy> Tower::findTarget(const std::vector<std::shared_ptr<Enemy>>& enemies) const {
    std::shared_ptr<Enemy> target = nullptr;
    float closestDistance = range * range; // Using squared distance for comparison
    
    for (const auto& enemy : enemies) {
        if (enemy->isDefeated() || enemy->hasReachedEnd()) continue;
        
        float distance = Vector2DistanceSqr(position, enemy->getPosition());
        if (distance <= range * range && distance < closestDistance) {
            closestDistance = distance;
            target = enemy;
        }
    }
    
    return target;
}

void Tower::onShoot(std::shared_ptr<Enemy> target) {
    // This will be handled by the GameModel
}

TowerType Tower::getType() const {
    return type;
}

Vector2 Tower::getPosition() const {
    return position;
}

float Tower::getRange() const {
    return range;
}

int Tower::getLevel() const {
    return level;
}

int Tower::getCost() const {
    return cost;
}

int Tower::getUpgradeCost() const {
    return upgradeCost;
}

float Tower::getSplashRadius() const {
    return splashRadius;
}

float Tower::getSlowFactor() const {
    return slowFactor;
}

float Tower::getSlowDuration() const {
    return slowDuration;
}

float Tower::getPoisonDamage() const {
    return poisonDamage;
}

float Tower::getPoisonDuration() const {
    return poisonDuration;
}
