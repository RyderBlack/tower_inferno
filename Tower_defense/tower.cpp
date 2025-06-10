#include "tower.h"
#include <cmath>

Tower::Tower(sf::Vector2i pos, TowerType t, int baseCost, int upgradePrice, int maxLvl, TowerStats baseStats)
    : position(pos),
      level(1),
      cost(baseCost),
      upgradeCost(upgradePrice),
      maxLevel(maxLvl),
      type(t),
      stats(),
      lastAttackTime(0.0f),
      baseStats(baseStats) {
    updateStatsForLevel();
}

void Tower::upgrade() {
    if (level < maxLevel) {
        level++;
        updateStatsForLevel();
    }
}

void Tower::updateStatsForLevel() {
    stats.damage = baseStats.damage * (1.0f + (level - 1) * 0.2f);
    stats.attackSpeed = baseStats.attackSpeed * (1.0f + (level - 1) * 0.1f);
    stats.range = baseStats.range * (1.0f + (level - 1) * 0.05f);
    stats.areaRadius = baseStats.areaRadius * (1.0f + (level - 1) * 0.1f);
    stats.effectDuration = baseStats.effectDuration * (1.0f + (level - 1) * 0.1f);
    stats.effectPower = baseStats.effectPower * (1.0f + (level - 1) * 0.1f);
}

bool Tower::canAttack(float currentTime) const {
    return (currentTime - lastAttackTime) >= (1.0f / stats.attackSpeed);
}

void Tower::attack(float currentTime) {
    lastAttackTime = currentTime;
}

int Tower::getSellPrice() const {
    return static_cast<int>(cost * 0.7f * level);
}

// Static member definitions
sf::Texture BasicTower::texture_harpoon1, BasicTower::texture_harpoon2, BasicTower::texture_harpoon3;
bool BasicTower::texturesLoaded = false;

BasicTower::BasicTower(sf::Vector2i pos)
    : Tower(pos, TowerType::BASIC, 100, 50, 5, getBaseStats()) {
    if (!texturesLoaded) {
        loadTextures();
    }
}

TowerStats BasicTower::getBaseStats() const {
    TowerStats stats;
    stats.damage = 10.0f;
    stats.attackSpeed = 1.0f;
    stats.range = 100.0f;
    stats.areaRadius = 0.0f;
    stats.effect = EffectType::NONE;
    stats.effectDuration = 0.0f;
    stats.effectPower = 0.0f;
    return stats;
}

void BasicTower::loadTextures() {
    if (!texturesLoaded) {
        texture_harpoon1.loadFromFile("assets/sprites/towers/harpoon1.png");
        texture_harpoon2.loadFromFile("assets/sprites/towers/harpoon2.png");
        texture_harpoon3.loadFromFile("assets/sprites/towers/harpoon3.png");
        texturesLoaded = true;
    }
}

void BasicTower::render(sf::RenderWindow& window, int tileSize) {
    sf::Sprite sprite;
    if (level <= 2) {
        sprite.setTexture(texture_harpoon1);
    } else if (level <= 4) {
        sprite.setTexture(texture_harpoon2);
    } else {
        sprite.setTexture(texture_harpoon3);
    }
    
    // Center the sprite on the tile
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2, bounds.height / 2);
    sprite.setPosition(position.x * tileSize + tileSize / 2, position.y * tileSize + tileSize / 2);
    
    // Scale down the sprite to fit the tile
    float scale = static_cast<float>(tileSize) * 0.8f / std::max(bounds.width, bounds.height);
    sprite.setScale(scale, scale);
    
    window.draw(sprite);
}

std::string BasicTower::getName() const {
    return "Harpoon Tower";
}

// FireTower implementation
sf::Texture FireTower::texture_fire1, FireTower::texture_fire2, FireTower::texture_fire3;
bool FireTower::texturesLoaded = false;

FireTower::FireTower(sf::Vector2i pos)
    : Tower(pos, TowerType::FIRE, 150, 75, 5, getBaseStats()) {
    if (!texturesLoaded) {
        loadTextures();
    }
}

TowerStats FireTower::getBaseStats() const {
    TowerStats stats;
    stats.damage = 5.0f;
    stats.attackSpeed = 2.0f;
    stats.range = 80.0f;
    stats.areaRadius = 40.0f;
    stats.effect = EffectType::BURN;
    stats.effectDuration = 3.0f;
    stats.effectPower = 5.0f;
    return stats;
}

void FireTower::loadTextures() {
    if (!texturesLoaded) {
        texture_fire1.loadFromFile("assets/sprites/towers/fire1.png");
        texture_fire2.loadFromFile("assets/sprites/towers/fire2.png");
        texture_fire3.loadFromFile("assets/sprites/towers/fire3.png");
        texturesLoaded = true;
    }
}

void FireTower::render(sf::RenderWindow& window, int tileSize) {
    sf::Sprite sprite;
    if (level <= 2) {
        sprite.setTexture(texture_fire1);
    } else if (level <= 4) {
        sprite.setTexture(texture_fire2);
    } else {
        sprite.setTexture(texture_fire3);
    }
    
    // Center the sprite on the tile
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2, bounds.height / 2);
    sprite.setPosition(position.x * tileSize + tileSize / 2, position.y * tileSize + tileSize / 2);
    
    // Scale down the sprite to fit the tile
    float scale = static_cast<float>(tileSize) * 0.8f / std::max(bounds.width, bounds.height);
    sprite.setScale(scale, scale);
    
    window.draw(sprite);
}

std::string FireTower::getName() const {
    return "Fire Tower";
}

// IceTower implementation
sf::Texture IceTower::texture_ice1, IceTower::texture_ice2, IceTower::texture_ice3;
bool IceTower::texturesLoaded = false;

IceTower::IceTower(sf::Vector2i pos)
    : Tower(pos, TowerType::ICE, 200, 100, 5, getBaseStats()) {
    if (!texturesLoaded) {
        loadTextures();
    }
}

TowerStats IceTower::getBaseStats() const {
    TowerStats stats;
    stats.damage = 3.0f;
    stats.attackSpeed = 0.5f;
    stats.range = 120.0f;
    stats.areaRadius = 60.0f;
    stats.effect = EffectType::FREEZE;
    stats.effectDuration = 5.0f;
    stats.effectPower = 0.5f; // 50% slow
    return stats;
}

void IceTower::loadTextures() {
    if (!texturesLoaded) {
        texture_ice1.loadFromFile("assets/sprites/towers/ice1.png");
        texture_ice2.loadFromFile("assets/sprites/towers/ice2.png");
        texture_ice3.loadFromFile("assets/sprites/towers/ice3.png");
        texturesLoaded = true;
    }
}

void IceTower::render(sf::RenderWindow& window, int tileSize) {
    sf::Sprite sprite;
    if (level <= 2) {
        sprite.setTexture(texture_ice1);
    } else if (level <= 4) {
        sprite.setTexture(texture_ice2);
    } else {
        sprite.setTexture(texture_ice3);
    }
    
    // Center the sprite on the tile
    sf::FloatRect bounds = sprite.getLocalBounds();
    sprite.setOrigin(bounds.width / 2, bounds.height / 2);
    sprite.setPosition(position.x * tileSize + tileSize / 2, position.y * tileSize + tileSize / 2);
    
    // Scale down the sprite to fit the tile
    float scale = static_cast<float>(tileSize) * 0.8f / std::max(bounds.width, bounds.height);
    sprite.setScale(scale, scale);
    
    window.draw(sprite);
}

std::string IceTower::getName() const {
    return "Ice Tower";
}

// TowerFactory implementation
std::unique_ptr<Tower> TowerFactory::createTower(TowerType type, sf::Vector2i position) {
    switch (type) {
        case TowerType::BASIC: return std::make_unique<BasicTower>(position);
        case TowerType::FIRE: return std::make_unique<FireTower>(position);
        case TowerType::ICE: return std::make_unique<IceTower>(position);
        default: return nullptr;
    }
}

std::vector<TowerType> TowerFactory::getAvailableTowers() {
    return {TowerType::BASIC, TowerType::FIRE, TowerType::ICE};
}

std::string TowerFactory::getTowerName(TowerType type) {
    switch (type) {
        case TowerType::BASIC: return "Harpoon Tower";
        case TowerType::FIRE: return "Fire Tower";
        case TowerType::ICE: return "Ice Tower";
        default: return "Unknown Tower";
    }
}

std::string TowerFactory::getTowerDescription(TowerType type) {
    switch (type) {
        case TowerType::BASIC: return "Basic tower with balanced stats";
        case TowerType::FIRE: return "Sets enemies on fire, dealing damage over time";
        case TowerType::ICE: return "Slows down enemies in an area";
        default: return "Unknown tower type";
    }
}

int TowerFactory::getTowerCost(TowerType type) {
    switch (type) {
        case TowerType::BASIC: return 100;
        case TowerType::FIRE: return 150;
        case TowerType::ICE: return 200;
        default: return 0;
    }
}

// TowerSelectionUI implementation
TowerSelectionUI::TowerSelectionUI(sf::Font* f) : font(f) {
    auto availableTowers = TowerFactory::getAvailableTowers();
    
    // Create buttons for each tower type
    for (auto type : availableTowers) {
        sf::RectangleShape button(sf::Vector2f(120, 40));
        button.setFillColor(sf::Color(100, 100, 100));
        button.setOutlineThickness(2);
        button.setOutlineColor(sf::Color::White);
        
        sf::Text text;
        text.setFont(*font);
        text.setString(TowerFactory::getTowerName(type));
        text.setCharacterSize(12);
        text.setFillColor(sf::Color::White);
        
        buttons.push_back(button);
        buttonTexts.push_back(text);
        towerTypes.push_back(type);
    }
    
    updatePositions();
}

void TowerSelectionUI::show(sf::Vector2f pos) {
    position = pos;
    visible = true;
    updatePositions();
}

void TowerSelectionUI::hide() {
    visible = false;
}

bool TowerSelectionUI::isVisible() const {
    return visible;
}

void TowerSelectionUI::updatePositions() {
    for (size_t i = 0; i < buttons.size(); ++i) {
        buttons[i].setPosition(position.x, position.y + i * 50);
        sf::FloatRect bounds = buttonTexts[i].getLocalBounds();
        buttonTexts[i].setPosition(position.x + 60 - bounds.width/2, position.y + i * 50 + 10);
    }
}

TowerType TowerSelectionUI::checkClick(sf::Vector2i mousePos) {
    if (!visible) return static_cast<TowerType>(-1);
    
    for (size_t i = 0; i < buttons.size(); ++i) {
        if (buttons[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            return towerTypes[i];
        }
    }
    return static_cast<TowerType>(-1);
}

void TowerSelectionUI::render(sf::RenderWindow& window) {
    if (!visible) return;
    
    for (const auto& button : buttons) {
        window.draw(button);
    }
    
    for (const auto& text : buttonTexts) {
        window.draw(text);
    }
}