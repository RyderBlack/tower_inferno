#pragma once
#include "raylib.h"

class Enemy {
public:
    Enemy(Vector3 position, float speed, float health);
    
    void Update(float deltaTime);
    void TakeDamage(float damage);
    bool IsAlive() const;
    Vector3 GetPosition() const;
    float GetHealthRatio() const;
    
private:
    Vector3 position;
    float speed;
    float health;
    float maxHealth;
    bool alive;
};
