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

enum class EffectType {
    NONE,
    BURN,
    FREEZE,
    STUN
};

struct TowerStats {
    float damage;
    float attackSpeed;
    float range;
    float areaRadius;
    EffectType effect;
    float effectDuration;
    float effectPower;
};

class Tower {
public:
    sf::Vector2i position;
    int level = 1;
    int cost;
    int upgradeCost;
    int maxLevel;
    TowerType type;
    TowerStats stats;
    float lastAttackTime = 0.0f;

    Tower(sf::Vector2i pos, TowerType t, int baseCost, int upgradePrice, int maxLvl, TowerStats baseStats);
    virtual ~Tower() = default;
        
    virtual void render(sf::RenderWindow& window, int tileSize) = 0;
    virtual void upgrade();
    virtual int getSellPrice() const;
    virtual std::string getName() const = 0;
    virtual TowerStats getBaseStats() const = 0;
    
    bool canAttack(float currentTime) const;
    void attack(float currentTime);
    float getAttackCooldown() const { return 1.0f / stats.attackSpeed; }
    
protected:
    TowerStats baseStats;
    virtual void updateStatsForLevel();
};

class BasicTower : public Tower {
private:
    static sf::Texture texture_harpoon1, texture_harpoon2, texture_harpoon3;
    static bool texturesLoaded;

public:
    BasicTower(sf::Vector2i pos);
    static void loadTextures();
    void render(sf::RenderWindow& window, int tileSize) override;
    std::string getName() const override;
    TowerStats getBaseStats() const override;
};

class FireTower : public Tower {
private:
    static sf::Texture texture_fire1, texture_fire2, texture_fire3;
    static bool texturesLoaded;

public:
    FireTower(sf::Vector2i pos);
    static void loadTextures();
    void render(sf::RenderWindow& window, int tileSize) override;
    std::string getName() const override;
    TowerStats getBaseStats() const override;
};

class IceTower : public Tower {
private:
    static sf::Texture texture_ice1, texture_ice2, texture_ice3;
    static bool texturesLoaded;

public:
    IceTower(sf::Vector2i pos);
    static void loadTextures();
    void render(sf::RenderWindow& window, int tileSize) override;
    std::string getName() const override;
    TowerStats getBaseStats() const override;
};

class TowerFactory {
public:
    static std::unique_ptr<Tower> createTower(TowerType type, sf::Vector2i position);
    static std::vector<TowerType> getAvailableTowers();
    static std::string getTowerName(TowerType type);
    static int getTowerCost(TowerType type);
    static std::string getTowerDescription(TowerType type);
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