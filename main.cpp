#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include "include/json.hpp"

using json = nlohmann::json;

/* ---------- JSON-помощники ---------- */
void saveScoreToJson(int newScore) {
    json data;
    std::ifstream inFile("score.json");
    if (inFile.is_open()) {
        inFile >> data;
        inFile.close();
    }
    data["scores"].push_back(newScore);
    std::ofstream outFile("score.json");
    outFile << data.dump(4);
}

std::vector<int> loadScoresFromJson() {
    std::vector<int> scores;
    json data;
    std::ifstream inFile("score.json");
    if (inFile.is_open()) {
        inFile >> data;
        if (data.contains("scores") && data["scores"].is_array())
            for (auto &s : data["scores"])
                scores.push_back(s.get<int>());
    }
    std::sort(scores.begin(), scores.end(), std::greater<>());
    return scores;
}
/* ------------------------------------ */

enum class GameState { MENU, LEVEL_SELECT, SCORE, PLAY_LEVEL1, EXIT };

int main() {
    sf::RenderWindow window(sf::VideoMode(1312, 736), "Tower Defense");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("assets/font.ttf")) {
        std::cerr << "Erreur de chargement de la police\n";
        return -1;
    }

    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/image/tower_inferno_cover.png")) {
        std::cerr << "Erreur de chargement de l'image de fond\n";
        return -1;
    }
    sf::Sprite backgroundSprite(backgroundTexture);

    sf::Texture playTexture, scoreTexture, quitTexture, volumeTexture;
    if (!playTexture.loadFromFile("assets/image/play_button.png") ||
        !scoreTexture.loadFromFile("assets/image/score_button.png") ||
        !quitTexture.loadFromFile("assets/image/quit_icon.png") ||
        !volumeTexture.loadFromFile("assets/image/volume_icon.png")) {
        std::cerr << "Erreur de chargement des icônes\n";
        return -1;
    }

    sf::Texture scoreBackgroundTexture;
    if (!scoreBackgroundTexture.loadFromFile("assets/image/tower_inferno_fon.png")) {
        std::cerr << "Erreur de chargement de l'image de fond des scores\n";
        return -1;
    }
    sf::Sprite scoreBackground(scoreBackgroundTexture);

    sf::Sprite playButton(playTexture), scoreButton(scoreTexture), quitButton(quitTexture), volumeButton(volumeTexture);
    playButton.setScale(0.5f, 0.5f);
    volumeButton.setScale(0.2f, 0.2f);
    quitButton.setScale(0.2f, 0.2f);

    bool soundEnabled = true;
    GameState state = GameState::MENU;

    sf::Text soundStateText("Son: Active", font, 20);
    soundStateText.setFillColor(sf::Color::White);

    float centerX = window.getSize().x / 2.f;
    float centerY = window.getSize().y / 2.f;

    playButton.setOrigin(playButton.getLocalBounds().width / 2, playButton.getLocalBounds().height / 2);
    playButton.setPosition(centerX, centerY + 90);

    scoreButton.setOrigin(scoreButton.getLocalBounds().width / 2, scoreButton.getLocalBounds().height / 2);
    scoreButton.setPosition(centerX, centerY + 290);

    volumeButton.setOrigin(volumeButton.getLocalBounds().width / 2, volumeButton.getLocalBounds().height / 2);
    volumeButton.setPosition(60, window.getSize().y - 60);

    quitButton.setOrigin(quitButton.getLocalBounds().width / 2, quitButton.getLocalBounds().height / 2);
    quitButton.setPosition(window.getSize().x - 60, window.getSize().y - 60);

    soundStateText.setOrigin(soundStateText.getLocalBounds().width / 2, 0);
    soundStateText.setPosition(60, window.getSize().y - 30);

    // Level Select
    sf::Text levelTitle("Selection du niveau", font, 36);
    levelTitle.setPosition(200, 50);
    std::vector<sf::RectangleShape> levelButtons;
    std::vector<sf::Text> levelTexts;

    for (int i = 0; i < 8; ++i) {
        sf::RectangleShape btn(sf::Vector2f(80, 50));
        btn.setPosition(80 + i * 85, 200);
        btn.setFillColor(sf::Color(255, 165, 0));
        levelButtons.push_back(btn);

        sf::Text txt(std::to_string(i + 1), font, 24);
        txt.setPosition(btn.getPosition().x + 25, 210);
        levelTexts.push_back(txt);
    }

    sf::Text backButton("Retour", font, 20);
    backButton.setPosition(100, 500);

    sf::Text levelSoundButton("Son: Active", font, 20);
    levelSoundButton.setPosition(600, 500);

    sf::Text scoreTitle("SCORE", font, 48);
    scoreTitle.setStyle(sf::Text::Bold);
    scoreTitle.setFillColor(sf::Color::White);

    sf::Text scoreBackButton("Retour", font, 24);
    scoreBackButton.setFillColor(sf::Color::White);

    sf::Music backgroundMusic;
    if (backgroundMusic.openFromFile("assets/background.mp3")) {
        backgroundMusic.setLoop(true);
        backgroundMusic.setVolume(50);
        backgroundMusic.play();
    }

    // Level 1
    sf::RectangleShape tower(sf::Vector2f(50, 100));
    tower.setFillColor(sf::Color::Blue);
    tower.setPosition(100, window.getSize().y - 150);

    sf::CircleShape enemy(20);
    enemy.setFillColor(sf::Color::Red);
    float enemyX = 0.f;
    float enemySpeed = 100.f;
    bool enemyAlive = true;
    int score = 0;

    sf::Text scoreText("Score: 0", font, 24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 40);

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mouse(sf::Mouse::getPosition(window));

                if (state == GameState::MENU) {
                    if (playButton.getGlobalBounds().contains(mouse)) {
                        score = 0;
                        enemyX = 0.f;
                        enemyAlive = true;
                        state = GameState::LEVEL_SELECT;
                    } else if (scoreButton.getGlobalBounds().contains(mouse)) {
                        state = GameState::SCORE;
                    } else if (volumeButton.getGlobalBounds().contains(mouse)) {
                        soundEnabled = !soundEnabled;
                        soundStateText.setString(soundEnabled ? "Son: Active" : "Son: Desactive");
                        soundStateText.setOrigin(soundStateText.getLocalBounds().width / 2, 0);
                        soundEnabled ? backgroundMusic.play() : backgroundMusic.pause();
                    } else if (quitButton.getGlobalBounds().contains(mouse)) {
                        window.close();
                    }
                } else if (state == GameState::LEVEL_SELECT) {
                    for (int i = 0; i < 8; ++i) {
                        if (levelButtons[i].getGlobalBounds().contains(mouse)) {
                            if (i == 0) {
                                score = 0;
                                enemyX = 0.f;
                                enemyAlive = true;
                                state = GameState::PLAY_LEVEL1;
                            }
                        }
                    }
                    if (backButton.getGlobalBounds().contains(mouse)) {
                        state = GameState::MENU;
                    }
                    if (levelSoundButton.getGlobalBounds().contains(mouse)) {
                        soundEnabled = !soundEnabled;
                        levelSoundButton.setString(soundEnabled ? "Son: Active" : "Son: Desactive");
                        soundEnabled ? backgroundMusic.play() : backgroundMusic.pause();
                    }
                } else if (state == GameState::SCORE) {
                    if (scoreBackButton.getGlobalBounds().contains(mouse)) {
                        state = GameState::MENU;
                    }
                }
            }

            if (state == GameState::PLAY_LEVEL1 && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                saveScoreToJson(score);
                state = GameState::MENU;
            }
        }

        float deltaTime = clock.restart().asSeconds();

        if (state == GameState::PLAY_LEVEL1) {
            enemyX += enemySpeed * deltaTime;
            if (enemyX > tower.getPosition().x && enemyAlive) {
                score += 10;
                enemyAlive = false;
            }
            if (enemyX > window.getSize().x) {
                enemyX = -enemy.getRadius() * 2;
                enemyAlive = true;
            }
            enemy.setPosition(enemyX, window.getSize().y - 100);
            scoreText.setString("Score: " + std::to_string(score));
        }

        window.clear();

        if (state == GameState::MENU) {
            window.draw(backgroundSprite);
            window.draw(playButton);
            window.draw(scoreButton);
            window.draw(quitButton);
            window.draw(volumeButton);
            window.draw(soundStateText);
        } else if (state == GameState::LEVEL_SELECT) {
            window.draw(levelTitle);
            for (int i = 0; i < 8; ++i) {
                window.draw(levelButtons[i]);
                window.draw(levelTexts[i]);
            }
            window.draw(backButton);
            window.draw(levelSoundButton);
        } else if (state == GameState::SCORE) {
            window.draw(scoreBackground);
            float midX = window.getSize().x / 2.f;
            scoreTitle.setPosition(midX - scoreTitle.getLocalBounds().width / 2.f, 220);// высота надп скор
            window.draw(scoreTitle);

            // sf::Text lastScore("Dernier score: " + std::to_string(score), font, 28);
            // lastScore.setFillColor(sf::Color::White);
            // lastScore.setPosition(midX - lastScore.getLocalBounds().width / 2.f, 280);
            // window.draw(lastScore);

            auto scores = loadScoresFromJson();
            for (int i = 0; i < std::min(5, (int)scores.size()); ++i) {
                sf::Text txt("Top " + std::to_string(i + 1) + ": " + std::to_string(scores[i]), font, 24);
                txt.setFillColor(sf::Color::White);
                txt.setPosition(midX - txt.getLocalBounds().width / 2.f, 330 + i * 40);
                window.draw(txt);
            }

            scoreBackButton.setPosition(midX - scoreBackButton.getLocalBounds().width / 2.f, window.getSize().y - 80);
            window.draw(scoreBackButton);
        } else if (state == GameState::PLAY_LEVEL1) {
            window.clear(sf::Color(50, 50, 50));
            window.draw(tower);
            window.draw(enemy);
            window.draw(scoreText);

            sf::Text backToMenu("Retour au menu (Esc)", font, 20);
            backToMenu.setPosition(10, 10);
            backToMenu.setFillColor(sf::Color::White);
            window.draw(backToMenu);
        }

        window.display();
    }

    return 0;
}
