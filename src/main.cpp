#include "controller/GameController.h"
#include "utils/ResourceManager.h"
#include <raylib.h>

int main() {
    // Initialize window
    const int screenWidth = 1280;
    const int screenHeight = 720;
    
    InitWindow(screenWidth, screenHeight, "Tower Inferno");
    SetTargetFPS(60);
    
    // Initialize audio
    InitAudioDevice();
    
    // Load resources
    if (!ResourceManager::getInstance().loadResources()) {
        TraceLog(LOG_ERROR, "Failed to load resources!");
        CloseWindow();
        return -1;
    }
    
    // Create game controller
    GameController gameController(screenWidth, screenHeight);
    
    // Main game loop
    while (!WindowShouldClose()) {
        // Update game state
        gameController.update(GetFrameTime());
        
        // Render
        BeginDrawing();
            ClearBackground(RAYWHITE);
            gameController.render();
        EndDrawing();
    }
    
    // Cleanup
    ResourceManager::getInstance().unloadResources();
    CloseAudioDevice();
    CloseWindow();
    
    return 0;
}
