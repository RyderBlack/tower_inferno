#pragma once
#include "raylib.h"
#include <vector>

class Enemy; // Forward declaration

class Tower {
public:
    Tower(Vector3 position, float range, float fireRate);
    
    void Update(float deltaTime, std::vector<Enemy*>& enemies);
    Vector3 GetPosition() const;
    
private:
    Vector3 position;
    float range;
    float fireRate;
    float timeSinceLastShot;
    
    Enemy* FindTarget(const std::vector<Enemy*>& enemies) const;
};
