#include "Enemy.h"

Enemy::Enemy(EnemyType type, const std::vector<Vector2>& path, float health, float speed, int bounty, int scoreValue)
    : type(type), path(path), maxHealth(health), health(health), speed(speed), 
      bounty(bounty), scoreValue(scoreValue), pathIndex(0), position(path[0]) {}

void Enemy::update(float deltaTime) {
    if (pathIndex >= path.size() - 1) return;
    
    Vector2 target = path[pathIndex + 1];
    Vector2 direction = Vector2Subtract(target, position);
    float distance = Vector2Length(direction);
    
    if (distance < 1.0f) {
        pathIndex++;
        if (pathIndex >= path.size() - 1) {
            // Reached the end of the path
            return;
        }
        target = path[pathIndex + 1];
        direction = Vector2Subtract(target, position);
        distance = Vector2Length(direction);
    }
    
    direction = Vector2Normalize(direction);
    position = Vector2Add(position, Vector2Scale(direction, speed * deltaTime));
}

void Enemy::takeDamage(float damage) {
    health -= damage;
}

bool Enemy::isDefeated() const {
    return health <= 0;
}

bool Enemy::hasReachedEnd() const {
    return pathIndex >= path.size() - 1;
}

EnemyType Enemy::getType() const {
    return type;
}

float Enemy::getHealth() const {
    return health;
}

float Enemy::getMaxHealth() const {
    return maxHealth;
}

Vector2 Enemy::getPosition() const {
    return position;
}

int Enemy::getBounty() const {
    return bounty;
}

int Enemy::getScoreValue() const {
    return scoreValue;
}

std::vector<Vector2> Enemy::getPath() const {
    return path;
}
