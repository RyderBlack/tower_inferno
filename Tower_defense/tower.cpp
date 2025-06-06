#include "tower.h"
#include <cmath>

Tower::Tower(sf::Vector2i pos, TowerType t, int baseCost, int upgradePrice, int maxLvl, TowerStats baseStats) 
    : position(pos), type(t), cost(baseCost), upgradeCost(upgradePrice), maxLevel(maxLvl), baseStats(baseStats) {
    updateStatsForLevel();
}

void Tower::upgrade() {
    if (level < maxLevel) {
        level++;
        updateStatsForLevel();
    }
}

void Tower::updateStatsForLevel() {
    float multiplier = 1.0f + (level - 1) * 0.3f;
    stats.damage = baseStats.damage * multiplier;
    stats.attackSpeed = baseStats.attackSpeed * (1.0f + (level - 1) * 0.2f);
    stats.range = baseStats.range * (1.0f + (level - 1) * 0.15f);
    stats.areaRadius = baseStats.areaRadius * (1.0f + (level - 1) * 0.1f);
    stats.effect = baseStats.effect;
    stats.effectDuration = baseStats.effectDuration * (1.0f + (level - 1) * 0.25f);
    stats.effectPower = baseStats.effectPower * (1.0f + (level - 1) * 0.2f);
}

bool Tower::canAttack(float currentTime) const {
    return (currentTime - lastAttackTime) >= getAttackCooldown();
}

void Tower::attack(float currentTime) {
    lastAttackTime = currentTime;
}

int Tower::getSellPrice() const {
    return cost / 2;
}

sf::Texture BasicTower::texture_harpoon1, BasicTower::texture_harpoon2, BasicTower::texture_harpoon3;
bool BasicTower::texturesLoaded = false;

BasicTower::BasicTower(sf::Vector2i pos) 
    : Tower(pos, TowerType::BASIC, 100, 75, 3, {25.0f, 2.5f, 120.0f, 0.0f, EffectType::NONE, 0.0f, 0.0f}) {
    loadTextures();
}

TowerStats BasicTower::getBaseStats() const {
    return baseStats;
}

void BasicTower::loadTextures() {
    if (!texturesLoaded) {
        texture_harpoon1.loadFromFile("assets/harpoon1.png");
        texture_harpoon2.loadFromFile("assets/harpoon2.png");
        texture_harpoon3.loadFromFile("assets/harpoon3.png");
        texturesLoaded = true;
    }
}

void BasicTower::render(sf::RenderWindow& window, int tileSize) {
    sf::Sprite sprite;
    if (level == 1) sprite.setTexture(texture_harpoon1);
    else if (level == 2) sprite.setTexture(texture_harpoon2);
    else sprite.setTexture(texture_harpoon3);

    sf::Vector2u texSize = sprite.getTexture()->getSize();
    sprite.setOrigin(texSize.x / 2.f, texSize.y);
    float scaleX = static_cast<float>(tileSize) / texSize.x;
    sprite.setScale(scaleX, scaleX);

    sprite.setPosition(position.x * tileSize + tileSize / 2.f, (position.y + 1) * tileSize);
    window.draw(sprite);
}

std::string BasicTower::getName() const {
    return "Basic Tower";
}

sf::Texture FireTower::texture_fire1, FireTower::texture_fire2, FireTower::texture_fire3;
bool FireTower::texturesLoaded = false;

FireTower::FireTower(sf::Vector2i pos) 
    : Tower(pos, TowerType::FIRE, 150, 100, 3, {80.0f, 0.8f, 100.0f, 60.0f, EffectType::BURN, 3.0f, 15.0f}) {
    loadTextures();
}

TowerStats FireTower::getBaseStats() const {
    return baseStats;
}

void FireTower::loadTextures() {
    if (!texturesLoaded) {
        texture_fire1.loadFromFile("assets/fire1.png");
        texture_fire2.loadFromFile("assets/fire2.png");
        texture_fire3.loadFromFile("assets/fire3.png");
        texturesLoaded = true;
    }
}

void FireTower::render(sf::RenderWindow& window, int tileSize) {
    sf::Sprite sprite;
    if (level == 1) sprite.setTexture(texture_fire1);
    else if (level == 2) sprite.setTexture(texture_fire2);
    else sprite.setTexture(texture_fire3);

    sf::Vector2u texSize = sprite.getTexture()->getSize();
    sprite.setOrigin(texSize.x / 2.f, texSize.y);
    float scaleX = static_cast<float>(tileSize) / texSize.x;
    sprite.setScale(scaleX, scaleX);

    sprite.setPosition(position.x * tileSize + tileSize / 2.f, (position.y + 1) * tileSize);
    window.draw(sprite);
}

std::string FireTower::getName() const {
    return "Fire Tower";
}

sf::Texture IceTower::texture_ice1, IceTower::texture_ice2, IceTower::texture_ice3;
bool IceTower::texturesLoaded = false;

IceTower::IceTower(sf::Vector2i pos) 
    : Tower(pos, TowerType::ICE, 125, 80, 3, {12.0f, 1.5f, 110.0f, 80.0f, EffectType::FREEZE, 4.0f, 0.7f}) {
    loadTextures();
}

TowerStats IceTower::getBaseStats() const {
    return baseStats;
}

void IceTower::loadTextures() {
    if (!texturesLoaded) {
        texture_ice1.loadFromFile("assets/ice1.png");
        texture_ice2.loadFromFile("assets/ice2.png");
        texture_ice3.loadFromFile("assets/ice3.png");
        texturesLoaded = true;
    }
}

void IceTower::render(sf::RenderWindow& window, int tileSize) {
    sf::Sprite sprite;
    if (level == 1) sprite.setTexture(texture_ice1);
    else if (level == 2) sprite.setTexture(texture_ice2);
    else sprite.setTexture(texture_ice3);

    sf::Vector2u texSize = sprite.getTexture()->getSize();
    sprite.setOrigin(texSize.x / 2.f, texSize.y);
    float scaleX = static_cast<float>(tileSize) / texSize.x;
    sprite.setScale(scaleX, scaleX);

    sprite.setPosition(position.x * tileSize + tileSize / 2.f, (position.y + 1) * tileSize);
    window.draw(sprite);
}

std::string IceTower::getName() const {
    return "Ice Tower";
}

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
        case TowerType::BASIC: return "Basic";
        case TowerType::FIRE: return "Fire";
        case TowerType::ICE: return "Ice";
        default: return "Unknown";
    }
}

std::string TowerFactory::getTowerDescription(TowerType type) {
    switch (type) {
        case TowerType::BASIC: return "Fast, Unique";
        case TowerType::FIRE: return "Burn Zone";
        case TowerType::ICE: return "Freeze Zone";
        default: return "";
    }
}

int TowerFactory::getTowerCost(TowerType type) {
    switch (type) {
        case TowerType::BASIC: return 100;
        case TowerType::FIRE: return 150;
        case TowerType::ICE: return 125;
        default: return 100;
    }
}

TowerSelectionUI::TowerSelectionUI(sf::Font* f) : font(f) {
    auto availableTowers = TowerFactory::getAvailableTowers();
    
    for (size_t i = 0; i < availableTowers.size(); ++i) {
        TowerType type = availableTowers[i];
        towerTypes.push_back(type);
        
        sf::RectangleShape button({180.f, 40.f});
        button.setFillColor(sf::Color(100, 150, 200));
        buttons.push_back(button);
        
        sf::Text text;
        text.setFont(*font);
        text.setCharacterSize(14);
        text.setFillColor(sf::Color::White);
        
        std::string buttonText = TowerFactory::getTowerName(type) + " (" + 
                                std::to_string(TowerFactory::getTowerCost(type)) + "g)\n" +
                                TowerFactory::getTowerDescription(type);
        text.setString(buttonText);
        buttonTexts.push_back(text);
    }
}

void TowerSelectionUI::show(sf::Vector2f pos) {
    visible = true;
    position = pos;
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
        buttons[i].setPosition(position.x, position.y + i * 45);
        buttonTexts[i].setPosition(position.x + 5, position.y + i * 45 + 5);
    }
}

TowerType TowerSelectionUI::checkClick(sf::Vector2i mousePos) {
    if (!visible) return TowerType::BASIC;
    
    for (size_t i = 0; i < buttons.size(); ++i) {
        if (buttons[i].getGlobalBounds().contains(static_cast<sf::Vector2f>(mousePos))) {
            return towerTypes[i];
        }
    }
    return TowerType::BASIC;
}

void TowerSelectionUI::render(sf::RenderWindow& window) {
    if (!visible) return;
    
    for (size_t i = 0; i < buttons.size(); ++i) {
        window.draw(buttons[i]);
        window.draw(buttonTexts[i]);
    }
}