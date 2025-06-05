#include "tower.h"

Tower::Tower(sf::Vector2i pos, TowerType t, int baseCost, int upgradePrice, int maxLvl) 
    : position(pos), type(t), cost(baseCost), upgradeCost(upgradePrice), maxLevel(maxLvl) {}

void Tower::upgrade() {
    if (level < maxLevel) level++;
}

int Tower::getSellPrice() const {
    return cost / 2;
}

sf::Texture BasicTower::texture1, BasicTower::texture2, BasicTower::texture3;
bool BasicTower::texturesLoaded = false;

sf::Texture FireTower::texture1, FireTower::texture2, FireTower::texture3;
bool FireTower::texturesLoaded = false;

sf::Texture IceTower::texture1, IceTower::texture2, IceTower::texture3;
bool IceTower::texturesLoaded = false;

BasicTower::BasicTower(sf::Vector2i pos) : Tower(pos, TowerType::BASIC, 100, 75, 3) {
    loadTextures();
}

void BasicTower::loadTextures() {
    if (!texturesLoaded) {
        texture1.loadFromFile("assets/Towers1.png");
        texture2.loadFromFile("assets/Towers2.png");
        texture3.loadFromFile("assets/Towers3.png");
        texturesLoaded = true;
    }
}

void BasicTower::render(sf::RenderWindow& window, int tileSize) {
    sf::Sprite sprite;
    if (level == 1) sprite.setTexture(texture1);
    else if (level == 2) sprite.setTexture(texture2);
    else sprite.setTexture(texture3);

    sf::Vector2u texSize = sprite.getTexture()->getSize();
    sprite.setOrigin(texSize.x / 2.f, texSize.y);
    float scaleX = static_cast<float>(tileSize) / texSize.x;
    sprite.setScale(scaleX, scaleX);

    sprite.setPosition(
        position.x * tileSize + tileSize / 2.f,
        (position.y + 1) * tileSize
    );

    window.draw(sprite);
}

std::string BasicTower::getName() const {
    return "Basic Tower";
}

FireTower::FireTower(sf::Vector2i pos) : Tower(pos, TowerType::FIRE, 150, 100, 3) {
    loadTextures();
}

void FireTower::loadTextures() {
    if (!texturesLoaded) {
        texture1.loadFromFile("assets/Towers1.png");
        texture2.loadFromFile("assets/Towers2.png");
        texture3.loadFromFile("assets/Towers3.png");
        texturesLoaded = true;
    }
}

void FireTower::render(sf::RenderWindow& window, int tileSize) {
    sf::Sprite sprite;
    if (level == 1) sprite.setTexture(texture1);
    else if (level == 2) sprite.setTexture(texture2);
    else sprite.setTexture(texture3);

    sprite.setColor(sf::Color(255, 150, 150));

    sf::Vector2u texSize = sprite.getTexture()->getSize();
    sprite.setOrigin(texSize.x / 2.f, texSize.y);
    float scaleX = static_cast<float>(tileSize) / texSize.x;
    sprite.setScale(scaleX, scaleX);

    sprite.setPosition(
        position.x * tileSize + tileSize / 2.f,
        (position.y + 1) * tileSize
    );

    window.draw(sprite);
}

std::string FireTower::getName() const {
    return "Fire Tower";
}

IceTower::IceTower(sf::Vector2i pos) : Tower(pos, TowerType::ICE, 125, 80, 3) {
    loadTextures();
}

void IceTower::loadTextures() {
    if (!texturesLoaded) {
        texture1.loadFromFile("assets/Towers1.png");
        texture2.loadFromFile("assets/Towers2.png");
        texture3.loadFromFile("assets/Towers3.png");
        texturesLoaded = true;
    }
}

void IceTower::render(sf::RenderWindow& window, int tileSize) {
    sf::Sprite sprite;
    if (level == 1) sprite.setTexture(texture1);
    else if (level == 2) sprite.setTexture(texture2);
    else sprite.setTexture(texture3);

    sprite.setColor(sf::Color(150, 150, 255));

    sf::Vector2u texSize = sprite.getTexture()->getSize();
    sprite.setOrigin(texSize.x / 2.f, texSize.y);
    float scaleX = static_cast<float>(tileSize) / texSize.x;
    sprite.setScale(scaleX, scaleX);

    sprite.setPosition(
        position.x * tileSize + tileSize / 2.f,
        (position.y + 1) * tileSize
    );

    window.draw(sprite);
}

std::string IceTower::getName() const {
    return "Ice Tower";
}

std::unique_ptr<Tower> TowerFactory::createTower(TowerType type, sf::Vector2i position) {
    switch (type) {
        case TowerType::BASIC:
            return std::make_unique<BasicTower>(position);
        case TowerType::FIRE:
            return std::make_unique<FireTower>(position);
        case TowerType::ICE:
            return std::make_unique<IceTower>(position);
        case TowerType::ELECTRIC:
            return nullptr;
        default:
            return nullptr;
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
        case TowerType::ELECTRIC: return "Electric";
        default: return "Unknown";
    }
}

int TowerFactory::getTowerCost(TowerType type) {
    switch (type) {
        case TowerType::BASIC: return 100;
        case TowerType::FIRE: return 150;
        case TowerType::ICE: return 125;
        case TowerType::ELECTRIC: return 200;
        default: return 100;
    }
}

TowerSelectionUI::TowerSelectionUI(sf::Font* f) : font(f) {
    auto availableTowers = TowerFactory::getAvailableTowers();
    
    for (size_t i = 0; i < availableTowers.size(); ++i) {
        TowerType type = availableTowers[i];
        towerTypes.push_back(type);
        
        sf::RectangleShape button({120.f, 30.f});
        button.setFillColor(sf::Color(100, 150, 200));
        buttons.push_back(button);
        
        sf::Text text;
        text.setFont(*font);
        text.setCharacterSize(16);
        text.setFillColor(sf::Color::White);
        text.setString(TowerFactory::getTowerName(type) + " (" + std::to_string(TowerFactory::getTowerCost(type)) + "g)");
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
        buttons[i].setPosition(position.x, position.y + i * 35);
        buttonTexts[i].setPosition(position.x + 5, position.y + i * 35 + 5);
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