#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 800), "Tower Defense");

    sf::Texture grassTexture;
    if (!grassTexture.loadFromFile("assets/grass_tile.png")) {
        std::cerr << "Erreur chargement grass_tile\n";
        return -1;
    }
    sf::Texture sandTexture;
    if (!sandTexture.loadFromFile("assets/sand_tile.png")) {
        std::cerr << "Erreur chargement sand_tile\n";
        return -1;
    }

    sf::Font font;
    if (!font.loadFromFile("assets/fast99.ttf")) {
        std::cerr << "Erreur : chargement police\n";
        return -1;
    }

    sf::Text goldText;
    goldText.setFont(font);
    goldText.setCharacterSize(30);
    goldText.setFillColor(sf::Color::Yellow);
    goldText.setPosition(70.f, 25.f); 

    sf::Texture goldTexture;
    if (!goldTexture.loadFromFile("assets/gold.png")) {
        std::cerr << "Erreur chargement gold.png\n";
        return -1;
    }
    sf::Sprite goldSprite;
    goldSprite.setTexture(goldTexture);
    goldSprite.setPosition(20.f, 20.f);

    sf::Texture heartTexture;
    if (!heartTexture.loadFromFile("assets/heart.png")) {
        std::cerr << "Erreur chargement heart.png" << std::endl;
        return -1;
    }

    int lives = 3;
    int gold = 200;
    const int towerCost = 100;
    const int upgradeCost = 75;

    const int rows = 20;
    const int cols = 20;
    const int tileSize = 40;
    int grid[rows][cols] = {0};

    std::vector<sf::Vector2i> path_1_1 = {
        {0,5},{1,5},{2,5},{3,5},{4,5},{5,5},{5,6},{5,7},{5,8},{5,9},{5,10},{6,10},
        {7,10},{8,10},{9,10},{10,10},{11,10},{12,10},{13,10},{14,10},{14,9},{14,8},
        {14,7},{14,6},{14,5},{14,4},{14,3},{15,3},{16,3},{17,3},{18,3},{19,3}
    };

    std::vector<sf::Vector2i> path_1_2 = {
        {0,16},{1,16},{2,16},{3,16},{4,16},{5,16},{6,16},{7,16},{8,16},{9,16},
        {9,15},{9,14},{9,13},{9,12},{9,11},{9,10}
    };

    for (auto& pos : path_1_1)
        grid[pos.y][pos.x] = 1;
    for (auto& pos : path_1_2)
        grid[pos.y][pos.x] = 1;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        goldText.setString(std::to_string(gold));

        window.clear(sf::Color::Black);

        for (int y = 0; y < rows; ++y) {
            for (int x = 0; x < cols; ++x) {
                sf::Sprite tileSprite;

                if (grid[y][x] == 1)
                    tileSprite.setTexture(sandTexture);
                else
                    tileSprite.setTexture(grassTexture);

                tileSprite.setPosition(x * tileSize, y * tileSize);
                window.draw(tileSprite);
            }
        }
        window.draw(goldSprite);
        window.draw(goldText);
            for (int i = 0; i < lives; ++i) {
            sf::Sprite heartSprite;
            heartSprite.setTexture(heartTexture);
            float spacing = 10.f;
            heartSprite.setPosition(window.getSize().x - (i + 1) * (heartSprite.getGlobalBounds().width + spacing), 10.f);
            window.draw(heartSprite);
        }

        window.display();
    }

    return 0;
}
