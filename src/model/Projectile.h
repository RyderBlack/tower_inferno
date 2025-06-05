#ifndef TOWER_INFERNO_PROJECTILE_H
#define TOWER_INFERNO_PROJECTILE_H

#include <raylib.h>

// Forward declaration
class Enemy;

class Projectile {
private:
    Vector2 position;
    Vector2 velocity;
    float speed;
    int damage;
    float lifetime;
    bool isActive;
    
    // For special effects
    float splashRadius;
    float slowAmount;
    float poisonDamage;
    int poisonDuration;
    
    // Reference to the target enemy
    Enemy* target;
    
public:
    Projectile(const Vector2& startPos, Enemy* target, float speed, int damage);
    
    void update(float deltaTime);
    void draw() const;
    
    // Getters
    bool getIsActive() const { return isActive; }
    int getDamage() const { return damage; }
    Enemy* getTarget() const { return target; }
    Vector2 getPosition() const { return position; }
    float getSplashRadius() const { return splashRadius; }
    float getSlowAmount() const { return slowAmount; }
    float getPoisonDamage() const { return poisonDamage; }
    int getPoisonDuration() const { return poisonDuration; }
    
    // Setters
    void setSplashRadius(float radius) { splashRadius = radius; }
    void setSlowAmount(float amount) { slowAmount = amount; }
    void setPoisonEffect(float damage, int duration) { 
        poisonDamage = damage; 
        poisonDuration = duration; 
    }
    
private:
    void move(float deltaTime);
    bool hasReachedTarget() const;
};

#endif //TOWER_INFERNO_PROJECTILE_H
