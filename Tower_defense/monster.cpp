#include "monster.h"
#include <cmath>
#include <iostream>

// Static member initialization
sf::Texture Monster::dolphinTexture1;
sf::Texture Monster::dolphinTexture2;
bool Monster::dolphinTexturesLoaded = false;

void Monster::loadDolphinTextures() {
    if (!dolphinTexturesLoaded) {
        if (!dolphinTexture1.loadFromFile("C:/Users/ryrym_i6sf5hg/CLionProjects/tower_inferno/assets/sprites/dolphin/dolphin_inferno_01.png")) {
            std::cerr << "Failed to load dolphin_inferno_01.png" << std::endl;
        }
        if (!dolphinTexture2.loadFromFile("C:/Users/ryrym_i6sf5hg/CLionProjects/tower_inferno/assets/sprites/dolphin/dolphin_inferno_02.png")) {
            std::cerr << "Failed to load dolphin_inferno_02.png" << std::endl;
        }
        dolphinTexturesLoaded = (dolphinTexture1.getSize().x > 0 && dolphinTexture2.getSize().x > 0);
    }
}

Monster::Monster(const Path* pathRef, MonsterType monsterType)
    : path(pathRef), currentWaypoint(0), health(100.0f), type(monsterType) {
    
    // Set speed based on monster type
    const float speedMultiplier = 60.0f; // Convert from per-frame to per-second
    switch (type) {
        case MonsterType::Dolphin:
            speed = 1.8f * speedMultiplier;
            if (dolphinTexturesLoaded) {
                sprite.setTexture(dolphinTexture1);
                sprite.setOrigin(dolphinTexture1.getSize().x / 2.0f, dolphinTexture1.getSize().y / 2.0f);
                sprite.setScale(0.1f, 0.1f); // Scale down to 10%
            } else {
                shape.setRadius(15.f);
                shape.setFillColor(sf::Color::Cyan);
                shape.setOrigin(15.f, 15.f);
            }
            break;
        case MonsterType::Blue:
            speed = 1.2f * speedMultiplier;
            shape.setRadius(12.0f);
            shape.setFillColor(sf::Color(0, 100, 255));
            shape.setOrigin(12.0f, 12.0f);
            break;
        case MonsterType::Green:
            speed = 1.6f * speedMultiplier;
            shape.setRadius(14.0f);
            shape.setFillColor(sf::Color(0, 200, 0));
            shape.setOrigin(14.0f, 14.0f);
            break;
        case MonsterType::Yellow:
            speed = 2.0f * speedMultiplier;
            shape.setRadius(16.0f);
            shape.setFillColor(sf::Color(255, 255, 0));
            shape.setOrigin(16.0f, 16.0f);
            break;
        case MonsterType::Purple:
            speed = 2.4f * speedMultiplier;
            shape.setRadius(18.0f);
            shape.setFillColor(sf::Color(160, 32, 240));
            shape.setOrigin(18.0f, 18.0f);
            break;
        case MonsterType::Red:
        default:
            speed = 2.8f * speedMultiplier;
            shape.setRadius(20.0f);
            shape.setFillColor(sf::Color(255, 50, 50));
            shape.setOrigin(20.0f, 20.0f);
            break;
    }
    
    // Start at the first waypoint
    if (path && !path->waypoints.empty()) {
        if (type == MonsterType::Dolphin && dolphinTexturesLoaded) {
            sprite.setPosition(path->waypoints[0]);
        } else {
            shape.setPosition(path->waypoints[0]);
        }
    }
}

void Monster::update(float deltaTime) {
    if (!path || currentWaypoint >= static_cast<int>(path->waypoints.size()) - 1) {
        return;
    }
    
    sf::Vector2f currentPos = getPosition();
    sf::Vector2f targetPos = path->waypoints[currentWaypoint + 1];
    
    sf::Vector2f direction = targetPos - currentPos;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    
    if (distance > 0) {
        direction /= distance;
    }
    
    if (distance < 5.0f) { 
        currentWaypoint++;
    } else {
        if (type == MonsterType::Dolphin && dolphinTexturesLoaded) {
            sprite.move(direction * speed * deltaTime);
            float angleRad = std::atan2(direction.y, direction.x);
            float angleDeg = angleRad * 180.0f / 3.14159265f;
            sprite.setRotation(angleDeg);
            
            // Animate dolphin
            dolphinAnimTimer += deltaTime;
            if (dolphinAnimTimer >= dolphinAnimInterval) {
                dolphinAnimFrame = 1 - dolphinAnimFrame;
                dolphinAnimTimer = 0.0f;
                if (dolphinAnimFrame == 0) {
                    sprite.setTexture(dolphinTexture1);
                } else {
                    sprite.setTexture(dolphinTexture2);
                }
            }
        } else {
            shape.move(direction * speed * deltaTime);
        }
    }
}

void Monster::draw(sf::RenderWindow& window) const {
    if (type == MonsterType::Dolphin && dolphinTexturesLoaded) {
        window.draw(sprite);
    } else {
        window.draw(shape);
    }
}

bool Monster::hasReachedEnd() const {
    return !path || currentWaypoint >= static_cast<int>(path->waypoints.size()) - 1;
}

const sf::Vector2f& Monster::getPosition() const {
    if (type == MonsterType::Dolphin && dolphinTexturesLoaded) {
        return sprite.getPosition();
    }
    return shape.getPosition();
}
