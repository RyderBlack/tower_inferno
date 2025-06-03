#include "Tower.h"
#include "Enemy.h"
#include <cmath>
#include "raymath.h"  // For Vector3Distance

Tower::Tower(Vector3 position, float range, float fireRate)
    : position(position), range(range), fireRate(fireRate), timeSinceLastShot(0) {}

void Tower::Update(float deltaTime, std::vector<Enemy*>& enemies) {
    timeSinceLastShot += deltaTime;
    if (timeSinceLastShot < 1.0f / fireRate) return;
    
    if (Enemy* target = FindTarget(enemies)) {
        target->TakeDamage(10.0f);
        timeSinceLastShot = 0;
    }
}

Vector3 Tower::GetPosition() const {
    return position;
}

Enemy* Tower::FindTarget(const std::vector<Enemy*>& enemies) const {
    Enemy* bestTarget = nullptr;
    float bestDistance = range;
    
    for (auto enemy : enemies) {
        if (!enemy->IsAlive()) continue;
        
        float distance = Vector3Distance(position, enemy->GetPosition());
        if (distance < bestDistance) {
            bestTarget = enemy;
            bestDistance = distance;
        }
    }
    
    return bestTarget;
}
