#include "GameView.h"
#include "../model/Enemy.h"
#include "../model/Tower.h"

void GameView::Initialize() {
    // Initialize any view-specific resources here
}

void GameView::Shutdown() {
    // Clean up any view-specific resources here
}

void GameView::Render(const Game& game, const Camera& camera) {
    BeginMode3D(camera);
    
    // Draw ground
    DrawPlane({0, 0, 0}, {18, 12}, LIGHTGRAY);
    
    // Draw tower
    RenderTower(game.GetTower());
    
    // Draw enemies
    for (const auto& enemy : game.GetEnemies()) {
        if (enemy->IsAlive()) {
            RenderEnemy(*enemy);
        }
    }
    
    EndMode3D();
}

void GameView::RenderUI(const Game& game) {
    // Draw score and wave counter
    DrawText(TextFormat("Score: %d", game.GetScore()), 20, 20, 24, BLACK);
    DrawText(TextFormat("Wave: %d", game.GetWave()), 20, 50, 24, BLACK);
    
    // Game over or victory message
    if (game.GetState() == Game::State::GAME_OVER) {
        const char* message = game.IsGameWon() ? "VICTORY!" : "DEFEAT!";
        Color color = game.IsGameWon() ? GREEN : RED;
        DrawText(message, 520, 250, 48, color);
        DrawText(TextFormat("Final Score: %d", game.GetScore()), 520, 320, 32, BLACK);
        DrawText("ENTER: Menu", 570, 380, 24, DARKGRAY);
    }
}

void GameView::RenderEnemy(const Enemy& enemy) {
    Vector3 pos = enemy.GetPosition();
    DrawSphere(pos, 0.3f, RED);
    RenderHealthBar({pos.x, pos.y + 0.6f, pos.z}, enemy.GetHealthRatio());
}

void GameView::RenderTower(const Tower& tower) {
    Vector3 pos = tower.GetPosition();
    DrawCube(pos, 0.5f, 1.0f, 0.5f, BLUE);
}

void GameView::RenderHealthBar(Vector3 position, float healthRatio) {
    float barWidth = 0.6f, barHeight = 0.1f;
    DrawCube({position.x, position.y, position.z}, barWidth, barHeight, 0.05f, DARKGRAY);
    DrawCube(
        {position.x - (1 - healthRatio) * barWidth / 2, position.y, position.z + 0.01f},
        barWidth * healthRatio, 
        barHeight * 0.8f, 
        0.06f, 
        LIME
    );
}
