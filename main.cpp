#include <SFML/Graphics.hpp>
#include <iostream>
#include <SFML/Audio.hpp>

enum class GameState
{
    MENU,
    LEVEL_SELECT,
    SCORE,
    PLAY_LEVEL1,
    EXIT
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(1312, 736), "Tower Defense");
    window.setFramerateLimit(60);

    sf::Font font;
    if (!font.loadFromFile("assets/font.ttf"))
    {
        std::cerr << "Erreur de chargement de la police\n";
        return -1;
    }

    // --- Background image ---
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("assets/image/tower_inferno_cover.png"))
    {
        std::cerr << "Erreur de chargement de l'image de fond\n";
        return -1;
    }
    sf::Sprite backgroundSprite(backgroundTexture);

    // --- Icons ---
    sf::Texture playTexture, scoreTexture, quitTexture, volumeTexture;
    if (!playTexture.loadFromFile("assets/image/play_button.png") ||
        !scoreTexture.loadFromFile("assets/image/score_button.png") ||
        !quitTexture.loadFromFile("assets/image/quit_icon.png") ||
        !volumeTexture.loadFromFile("assets/image/volume_icon.png"))
    {
        std::cerr << "Erreur de chargement des icônes\n";
        return -1;
    }

    sf::Sprite playButton(playTexture);
    sf::Sprite scoreButtonSprite(scoreTexture);
    sf::Sprite quitButtonSprite(quitTexture);
    sf::Sprite volumeButton(volumeTexture);

    playButton.setScale(0.5f, 0.5f);
    volumeButton.setScale(0.2f, 0.2f);
    quitButtonSprite.setScale(0.2f, 0.2f);

    bool soundEnabled = true;
    GameState state = GameState::MENU;

    sf::Text soundStateText("", font, 20);
    soundStateText.setFillColor(sf::Color::White);

    // Center buttons and set origin after scaling
    float centerX = window.getSize().x / 2.0f;
    float centerY = window.getSize().y / 2.0f;

    playButton.setOrigin(playButton.getLocalBounds().width / 2, playButton.getLocalBounds().height / 2);
    playButton.setPosition(centerX, centerY + 90);

    scoreButtonSprite.setOrigin(scoreButtonSprite.getLocalBounds().width / 2, scoreButtonSprite.getLocalBounds().height / 2);
    scoreButtonSprite.setPosition(centerX, centerY + 290);

    volumeButton.setOrigin(volumeButton.getLocalBounds().width / 2, volumeButton.getLocalBounds().height / 2);
    volumeButton.setPosition(60, window.getSize().y - 60);

    quitButtonSprite.setOrigin(quitButtonSprite.getLocalBounds().width / 2, quitButtonSprite.getLocalBounds().height / 2);
    quitButtonSprite.setPosition(window.getSize().x - 60, window.getSize().y - 60);

    soundStateText.setString(soundEnabled ? "Son: Active" : "Son: Desactive");
    soundStateText.setOrigin(soundStateText.getLocalBounds().width / 2, 0);
    soundStateText.setPosition(60, window.getSize().y - 30);

    // --- Level select ---
    sf::Text levelTitle("Selection du niveau", font, 36);
    levelTitle.setPosition(200, 50);

    std::vector<sf::RectangleShape> levelButtons;
    std::vector<sf::Text> levelTexts;
    for (int i = 0; i < 8; ++i)
    {
        sf::RectangleShape btn(sf::Vector2f(80, 50));
        btn.setPosition(80 + i * 85, 200);
        btn.setFillColor(sf::Color(255, 165, 0)); // Orange
        levelButtons.push_back(btn);

        sf::Text txt(std::to_string(i + 1), font, 24);
        txt.setPosition(btn.getPosition().x + 25, 210);
        levelTexts.push_back(txt);
    }

    sf::Text backButton("Retour", font, 20);
    backButton.setPosition(100, 500);

    sf::Text levelSoundButton("Son: Active", font, 20);
    levelSoundButton.setPosition(600, 500);

    // --- Score screen ---
    sf::Text scoreTitle(" SCORE", font, 36);
    scoreTitle.setPosition(320, 50);

    int currentScore = 12345;
    sf::Text scoreValue("Score actuel: " + std::to_string(currentScore), font, 24);
    scoreValue.setPosition(280, 150);

    sf::Text scoreBackButton(" Retour", font, 20);
    scoreBackButton.setPosition(350, 300);

    // --- Music ---
    sf::Music backgroundMusic;
    if (!backgroundMusic.openFromFile("assets/background.mp3"))
    {
        std::cerr << "Erreur de chargement de la musique !" << std::endl;
    }
    else
    {
        backgroundMusic.setLoop(true);
        backgroundMusic.setVolume(50);
        backgroundMusic.play();
    }

    // --- Level 1 variables ---
    sf::RectangleShape tower(sf::Vector2f(50, 100));
    tower.setFillColor(sf::Color::Blue);
    tower.setPosition(100, window.getSize().y - 150);

    sf::CircleShape enemy(20);
    float enemyX = 0.f;
    float enemySpeed = 100.f; // pixels per second
    enemy.setFillColor(sf::Color::Red);
    enemy.setPosition(enemyX, window.getSize().y - 100);

    sf::Clock clock;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed &&
                event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mouse(sf::Mouse::getPosition(window));

                if (state == GameState::MENU)
                {
                    if (playButton.getGlobalBounds().contains(mouse))
                    {
                        state = GameState::LEVEL_SELECT;
                    }
                    else if (scoreButtonSprite.getGlobalBounds().contains(mouse))
                    {
                        state = GameState::SCORE;
                    }
                    else if (volumeButton.getGlobalBounds().contains(mouse))
                    {
                        soundEnabled = !soundEnabled;
                        soundStateText.setString(soundEnabled ? "Son: Active" : "Son: Desactive");
                        soundStateText.setOrigin(soundStateText.getLocalBounds().width / 2, 0);
                        if (soundEnabled)
                            backgroundMusic.play();
                        else
                            backgroundMusic.pause();
                    }
                    else if (quitButtonSprite.getGlobalBounds().contains(mouse))
                    {
                        window.close();
                    }
                }
                else if (state == GameState::LEVEL_SELECT)
                {
                    for (int i = 0; i < 8; ++i)
                    {
                        if (levelButtons[i].getGlobalBounds().contains(mouse))
                        {
                            if (i == 0) // Level 1
                            {
                                state = GameState::PLAY_LEVEL1;
                            }
                            else
                            {
                                std::cout << "Lancement du niveau " << i + 1 << "\n";
                            }
                        }
                    }

                    if (backButton.getGlobalBounds().contains(mouse))
                    {
                        state = GameState::MENU;
                    }

                    if (levelSoundButton.getGlobalBounds().contains(mouse))
                    {
                        soundEnabled = !soundEnabled;
                        levelSoundButton.setString(soundEnabled ? "Son: Active" : "Son: Desactive");

                        if (soundEnabled)
                            backgroundMusic.play();
                        else
                            backgroundMusic.pause();
                    }
                }
                else if (state == GameState::SCORE)
                {
                    if (scoreBackButton.getGlobalBounds().contains(mouse))
                    {
                        state = GameState::MENU;
                    }
                }
            }

            // Обработка выхода из уровня нажатием ESC
            if (state == GameState::PLAY_LEVEL1)
            {
                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
                {
                    state = GameState::MENU;
                }
            }
        }

        float deltaTime = clock.restart().asSeconds();

        // Обновления для уровня 1
        if (state == GameState::PLAY_LEVEL1)
        {
            enemyX += enemySpeed * deltaTime;
            if (enemyX > window.getSize().x)
                enemyX = -enemy.getRadius() * 2;

            enemy.setPosition(enemyX, window.getSize().y - 100);
        }

        // --- Отрисовка ---
        window.clear(sf::Color(30, 30, 30));

        if (state == GameState::MENU)
        {
            window.draw(backgroundSprite);
            window.draw(playButton);
            window.draw(scoreButtonSprite);
            window.draw(quitButtonSprite);
            window.draw(volumeButton);
            window.draw(soundStateText);
        }
        else if (state == GameState::LEVEL_SELECT)
        {
            window.draw(levelTitle);
            for (int i = 0; i < 8; ++i)
            {
                window.draw(levelButtons[i]);
                window.draw(levelTexts[i]);
            }
            window.draw(backButton);
            window.draw(levelSoundButton);
        }
        else if (state == GameState::SCORE)
        {
            window.draw(scoreTitle);
            window.draw(scoreValue);
            window.draw(scoreBackButton);
        }
        else if (state == GameState::PLAY_LEVEL1)
        {
            window.clear(sf::Color(50, 50, 50));
            window.draw(tower);
            window.draw(enemy);

            sf::Text backToMenu("Retour au menu (Esc)", font, 20);
            backToMenu.setPosition(10, 10);
            backToMenu.setFillColor(sf::Color::White);
            window.draw(backToMenu);
        }

        window.display();
    }

    return 0;
}
