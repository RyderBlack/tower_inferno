
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include <string>

enum class TowerType {
    BASIC,
    FIRE,
    ICE,
    ELECTRIC
};

class Tower {
public:
    sf::Vector2i position;
    int level = 1;
    int cost;
    int upgradeCost;
    int maxLevel;
    TowerType type;

    Tower(sf::Vector2i pos, TowerType t, int baseCost, int upgradePrice, int maxLvl);
    virtual ~Tower() = default;
    
    virtual void render(sf::RenderWindow& window, int tileSize) = 0;
    virtual void upgrade();
    virtual int getSellPrice() const;
    virtual std::string getName() const = 0;
};

class BasicTower : public Tower {
private:
    static sf::Texture texture1, texture2, texture3;
    static bool texturesLoaded;

public:
    BasicTower(sf::Vector2i pos);
    static void loadTextures();
    void render(sf::RenderWindow& window, int tileSize) override;
    std::string getName() const override;
};

class FireTower : public Tower {
private:
    static sf::Texture texture1, texture2, texture3;
    static bool texturesLoaded;

public:
    FireTower(sf::Vector2i pos);
    static void loadTextures();
    void render(sf::RenderWindow& window, int tileSize) override;
    std::string getName() const override;
};

class IceTower : public Tower {
private:
    static sf::Texture texture1, texture2, texture3;
    static bool texturesLoaded;

public:
    IceTower(sf::Vector2i pos);
    static void loadTextures();
    void render(sf::RenderWindow& window, int tileSize) override;
    std::string getName() const override;
};

class TowerFactory {
public:
    static std::unique_ptr<Tower> createTower(TowerType type, sf::Vector2i position);
    static std::vector<TowerType> getAvailableTowers();
    static std::string getTowerName(TowerType type);
    static int getTowerCost(TowerType type);
};

class TowerSelectionUI {
private:
    std::vector<sf::RectangleShape> buttons;
    std::vector<sf::Text> buttonTexts;
    std::vector<TowerType> towerTypes;
    sf::Font* font;
    bool visible = false;
    sf::Vector2f position;

public:
    TowerSelectionUI(sf::Font* f);
    void show(sf::Vector2f pos);
    void hide();
    bool isVisible() const;
    void updatePositions();
    TowerType checkClick(sf::Vector2i mousePos);
    void render(sf::RenderWindow& window);
};