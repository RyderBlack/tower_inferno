#include "Projectile.h"

Projectile::Projectile(ProjectileType type, Vector2 startPos, std::shared_ptr<Enemy> target, 
                       float speed, float damage, Tower* sourceTower)
    : type(type), position(startPos), target(target), speed(speed), 
      damage(damage), sourceTower(sourceTower), timeToLive(5.0f), shouldRemoveFlag(false) {
    // Initialize any additional properties based on type
    switch (type) {
        case ProjectileType::BASIC:
            radius = 5.0f;
            break;
        case ProjectileType::SNIPER:
            radius = 3.0f;
            break;
        case ProjectileType::SPLASH:
            radius = 8.0f;
            break;
        case ProjectileType::SLOW:
            radius = 6.0f;
            break;
        case ProjectileType::POISON:
            radius = 4.0f;
            break;
    }
}

void Projectile::update(float deltaTime) {
    if (shouldRemoveFlag) return;
    
    timeToLive -= deltaTime;
    if (timeToLive <= 0.0f) {
        shouldRemoveFlag = true;
        return;
    }
    
    if (target && !target->isDefeated() && !target->hasReachedEnd()) {
        // Move towards target
        Vector2 direction = Vector2Subtract(target->getPosition(), position);
        float distance = Vector2Length(direction);
        
        if (distance <= radius + 5.0f) { // Hit target
            onHit();
            shouldRemoveFlag = true;
            return;
        }
        
        // Normalize and scale by speed
        direction = Vector2Scale(Vector2Normalize(direction), speed * deltaTime);
        position = Vector2Add(position, direction);
    } else {
        // Target is gone, remove this projectile
        shouldRemoveFlag = true;
    }
}

void Projectile::onHit() {
    if (!target || target->isDefeated() || target->hasReachedEnd()) return;
    
    // Apply damage to the target
    target->takeDamage(damage);
    
    // Apply special effects based on projectile type
    if (auto tower = sourceTower.lock()) {
        switch (type) {
            case ProjectileType::SPLASH: {
                // Apply splash damage to nearby enemies
                // This would be handled by the GameModel
                break;
            }
            case ProjectileType::SLOW: {
                // Apply slow effect
                // This would be handled by the GameModel
                break;
            }
            case ProjectileType::POISON: {
                // Apply poison effect
                // This would be handled by the GameModel
                break;
            }
            default:
                break;
        }
    }
}

bool Projectile::shouldRemove() const {
    return shouldRemoveFlag || timeToLive <= 0.0f;
}

Vector2 Projectile::getPosition() const {
    return position;
}

float Projectile::getRadius() const {
    return radius;
}

ProjectileType Projectile::getType() const {
    return type;
}

std::shared_ptr<Enemy> Projectile::getTarget() const {
    return target;
}
