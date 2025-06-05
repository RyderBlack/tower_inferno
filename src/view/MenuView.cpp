#include "MenuView.h"
#include "raylib.h"
#include "../utils/ResourceManager.h"

MenuView::MenuView() : 
    currentScreen(MenuScreen::MAIN_MENU),
    selectedButton(0),
    buttonCount(0) {
    // Initialize buttons
    initButtons();
}

void MenuView::update() {
    // Handle input for menu navigation
    if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
        selectedButton = (selectedButton + 1) % buttonCount;
        PlaySound(ResourceManager::getInstance().getSound("menu_select"));
    } else if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
        selectedButton = (selectedButton - 1 + buttonCount) % buttonCount;
        PlaySound(ResourceManager::getInstance().getSound("menu_select"));
    } else if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        PlaySound(ResourceManager::getInstance().getSound("menu_confirm"));
        handleButtonPress(selectedButton);
    } else if (IsKeyPressed(KEY_ESCAPE)) {
        if (currentScreen == MenuScreen::MAIN_MENU) {
            // Exit game if on main menu
            shouldClose = true;
        } else {
            // Go back to previous screen
            currentScreen = MenuScreen::MAIN_MENU;
            initButtons();
        }
    }
}

void MenuView::render() const {
    // Draw background
    ClearBackground(RAYWHITE);
    
    // Draw title
    const char* title = "TOWER INFERNO";
    int titleFontSize = 60;
    int titleY = 100;
    
    // Draw title shadow
    DrawText(title, (GetScreenWidth() - MeasureText(title, titleFontSize)) / 2 + 5, 
             titleY + 5, titleFontSize, GRAY);
    
    // Draw title
    DrawText(title, (GetScreenWidth() - MeasureText(title, titleFontSize)) / 2, 
             titleY, titleFontSize, DARKGRAY);
    
    // Draw buttons
    for (int i = 0; i < buttonCount; i++) {
        const auto& button = buttons[i];
        
        // Button background
        Color bgColor = (i == selectedButton) ? SKYBLUE : LIGHTGRAY;
        DrawRectangleRec(button.bounds, bgColor);
        
        // Button border
        DrawRectangleLinesEx(button.bounds, 2, DARKGRAY);
        
        // Button text
        int textWidth = MeasureText(button.text.c_str(), 24);
        DrawText(button.text.c_str(), 
                button.bounds.x + (button.bounds.width - textWidth) / 2,
                button.bounds.y + button.bounds.height / 2 - 12,
                24, DARKGRAY);
    }
    
    // Draw version info
    const char* version = "v1.0";
    DrawText(version, GetScreenWidth() - MeasureText(version, 20) - 20, 
             GetScreenHeight() - 30, 20, LIGHTGRAY);
}

void MenuView::initButtons() {
    buttons.clear();
    
    switch (currentScreen) {
        case MenuScreen::MAIN_MENU:
            addButton("START GAME", [this]() { 
                gameState = GameState::PLAYING;
            });
            addButton("OPTIONS", [this]() { 
                currentScreen = MenuScreen::OPTIONS;
                initButtons();
            });
            addButton("CREDITS", [this]() {
                currentScreen = MenuScreen::CREDITS;
                initButtons();
            });
            addButton("QUIT", [this]() { 
                shouldClose = true;
            });
            break;
            
        case MenuScreen::OPTIONS:
            // Add options menu buttons
            addButton("BACK", [this]() { 
                currentScreen = MenuScreen::MAIN_MENU;
                initButtons();
            });
            // Add more options here
            break;
            
        case MenuScreen::CREDITS:
            addButton("BACK", [this]() { 
                currentScreen = MenuScreen::MAIN_MENU;
                initButtons();
            });
            // Add credits text
            break;
            
        case MenuScreen::GAME_OVER:
            addButton("MAIN MENU", [this]() { 
                currentScreen = MenuScreen::MAIN_MENU;
                initButtons();
            });
            addButton("QUIT", [this]() { 
                shouldClose = true;
            });
            break;
    }
    
    selectedButton = 0;
}

void MenuView::addButton(const std::string& text, std::function<void()> action) {
    int buttonWidth = 200;
    int buttonHeight = 50;
    int startY = 250;
    int padding = 15;
    
    Button button{
        .text = text,
        .action = action,
        .bounds = {
            .x = static_cast<float>(GetScreenWidth() / 2 - buttonWidth / 2),
            .y = static_cast<float>(startY + (buttonHeight + padding) * buttonCount),
            .width = static_cast<float>(buttonWidth),
            .height = static_cast<float>(buttonHeight)
        }
    };
    
    buttons.push_back(button);
    buttonCount++;
}

void MenuView::handleButtonPress(int buttonIndex) {
    if (buttonIndex >= 0 && buttonIndex < buttonCount) {
        buttons[buttonIndex].action();
    }
}

bool MenuView::shouldGameClose() const {
    return shouldClose || WindowShouldClose();
}

GameState MenuView::getGameState() const {
    return gameState;
}

void MenuView::setGameState(GameState state) {
    gameState = state;
    if (state == GameState::GAME_OVER) {
        currentScreen = MenuScreen::GAME_OVER;
        initButtons();
    }
}
