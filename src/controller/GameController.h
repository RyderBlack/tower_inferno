#pragma once
#include "../model/Game.h"
#include "../view/GameView.h"
#include "../view/MenuView.h"
#include "InputManager.h"

class GameController {
public:
    GameController();
    ~GameController();
    
    void Initialize();
    void Run();
    void Shutdown();
    
private:
    Game game;
    GameView gameView;
    MenuView menuView;
    Camera camera;
    Music music;
    
    void ProcessInput();
    void Update(float deltaTime);
    void Render();
    
    // Input handling
    void HandleMenuInput();
    void HandleGameInput();
    void HandleGameOverInput();
};
