#ifndef TOWER_INFERNO_GAMECONTROLLER_H
#define TOWER_INFERNO_GAMECONTROLLER_H

#include "../model/GameModel.h"
#include "../view/GameView.h"
#include "../view/MenuView.h"
#include "../utils/Observable.h"

class GameController : public Observer {
private:
    // Game components
    GameModel model;
    GameView view;
    MenuView menuView;
    
    // Game state
    bool isPaused;
    bool isGameOver;
    bool isGameWon;
    
    // Input state
    Vector2 lastMousePosition;
    bool isDragging;
    
public:
    explicit GameController(int screenWidth, int screenHeight);
    ~GameController() override;
    
    // Main game loop methods
    void update(float deltaTime);
    void render();
    
    // Input handling
    void handleInput();
    
    // Game state management
    void startNewGame();
    void pauseGame();
    void resumeGame();
    void endGame(bool won);
    void togglePause();
    
    // Observer pattern
    void onNotify(int eventType, void* subject) override;
    
    // Getters
    bool getIsPaused() const { return isPaused; }
    bool getIsGameOver() const { return isGameOver; }
    bool getIsGameWon() const { return isGameWon; }
    
private:
    // Initialization
    void initializeGame();
    
    // Input helpers
    void handleGameInput();
    void handleMenuInput();
    void handleMouseInput();
    void handleKeyboardInput();
    void handleGamepadInput();
    
    // Game actions
    void placeTower(const Vector2& position, TowerType type);
    void sellTower(const Vector2& position);
    void upgradeTower(const Vector2& position);
    
    // UI actions
    void showMenu();
    void showOptions();
    void showCredits();
    void quitGame();
};

#endif //TOWER_INFERNO_GAMECONTROLLER_H
