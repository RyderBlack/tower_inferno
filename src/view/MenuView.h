#ifndef TOWER_INFERNO_MENUVIEW_H
#define TOWER_INFERNO_MENUVIEW_H

#include <raylib.h>
#include <string>
#include <vector>

// Forward declaration
class GameController;

struct MenuButton {
    Rectangle bounds;
    std::string text;
    int action;  // Action to perform when clicked
    Color color;
    Color hoverColor;
    bool isHovered;
};

class MenuView {
private:
    // Screen state
    enum class Screen {
        MAIN_MENU,
        GAME,
        PAUSE,
        OPTIONS,
        CREDITS,
        GAME_OVER,
        VICTORY
    };
    
    Screen currentScreen;
    
    // UI elements
    std::vector<MenuButton> mainMenuButtons;
    std::vector<MenuButton> pauseMenuButtons;
    std::vector<MenuButton> optionsButtons;
    std::vector<MenuButton> gameOverButtons;
    
    // Visual elements
    Texture2D background;
    Texture2D logo;
    Font titleFont;
    Font buttonFont;
    
    // Game controller reference
    GameController& controller;
    
    // Options state
    float musicVolume;
    float soundVolume;
    int difficulty;  // 0 = Easy, 1 = Normal, 2 = Hard
    
public:
    explicit MenuView(GameController& controller);
    ~MenuView();
    
    // Main rendering method
    void render();
    
    // Input handling
    void handleInput();
    
    // Screen management
    void showScreen(Screen screen);
    void showGameScreen() { showScreen(Screen::GAME); }
    void showPauseMenu() { showScreen(Screen::PAUSE); }
    void showMainMenu() { showScreen(Screen::MAIN_MENU); }
    void showOptions() { showScreen(Screen::OPTIONS); }
    void showCredits() { showScreen(Screen::CREDITS); }
    void showGameOver() { showScreen(Screen::GAME_OVER); }
    void showVictory() { showScreen(Screen::VICTORY); }
    
    // Getters
    bool isGameActive() const { return currentScreen == Screen::GAME; }
    
private:
    // Initialization
    void initializeUI();
    void loadResources();
    void unloadResources();
    
    // Rendering methods
    void renderMainMenu();
    void renderPauseMenu();
    void renderOptions();
    void renderCredits();
    void renderGameOver();
    void renderVictory();
    
    // UI helpers
    void drawButton(MenuButton& button, bool drawText = true);
    bool isButtonHovered(const MenuButton& button) const;
    void updateButtonHoverState(MenuButton& button);
    
    // Button actions
    void executeButtonAction(int action);
    
    // Option helpers
    void updateVolume();
    void updateDifficulty();
    
    // Slider helpers
    void drawSlider(const char* text, float x, float y, float width, float& value, float min, float max);
    bool isSliderHovered(float x, float y, float width) const;
};

#endif //TOWER_INFERNO_MENUVIEW_H
