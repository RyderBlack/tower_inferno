#include "Enemy.h"
#include <cmath>

Enemy::Enemy(Vector3 position, float speed, float health)
    : position(position), speed(speed), health(health), maxHealth(health), alive(true) {}

void Enemy::Update(float deltaTime) {
    if (!alive) return;
    
    Vector3 target = {8.0f, 0.3f, position.z};
    Vector3 dir = {target.x - position.x, 0, target.z - position.z};
    float len = std::sqrt(dir.x*dir.x + dir.z*dir.z);
    
    if (len > 0.1f) {
        dir.x /= len;
        dir.z /= len;
        position.x += dir.x * speed * deltaTime;
        position.z += dir.z * speed * deltaTime;
    } else {
        alive = false;
    }
}

void Enemy::TakeDamage(float damage) {
    health -= damage;
    if (health <= 0) {
        health = 0;
        alive = false;
    }
}

bool Enemy::IsAlive() const {
    return alive;
}

Vector3 Enemy::GetPosition() const {
    return position;
}

float Enemy::GetHealthRatio() const {
    return health / maxHealth;
}
