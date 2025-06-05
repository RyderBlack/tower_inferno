#include "GameView.h"
#include "../model/GameModel.h"
#include "../model/Enemy.h"
#include "../model/Tower.h"
#include "../model/Projectile.h"
#include <raylib.h>
#include <string>
#include <algorithm>
#include <cmath>

GameView::GameView(const GameModel& model) 
    : model(model), showTowerRange(false), selectedTowerType(TowerType::BASIC) {
    
    // Initialize camera
    camera = { 0 };
    camera.zoom = 1.0f;
    
    // Initialize UI elements
    sidebar = { (float)GetScreenWidth() - 200, 0, 200, (float)GetScreenHeight() };
    buildMenu = { 10, 10, 150, 300 };
    towerInfoPanel = { 10, (float)GetScreenHeight() - 150, 200, 140 };
    
    // Load default font
    defaultFont = GetFontDefault();
    titleFont = LoadFontEx("assets/fonts/your_font.ttf", 32, 0, 0);
    if (titleFont.texture.id == 0) {
        titleFont = defaultFont; // Fallback to default font
    }
    
    // Set default colors
    SetTargetFPS(60);
}

GameView::~GameView() {
    // Unload any loaded resources
    if (titleFont.texture.id != 0 && titleFont.texture.id != defaultFont.texture.id) {
        UnloadFont(titleFont);
    }
}

void GameView::onNotify(int eventType, void* subject) {
    // Handle game events
    switch (static_cast<GameEvent>(eventType)) {
        case GameEvent::ENEMY_DIED:
        case GameEvent::TOWER_PLACED:
        case GameEvent::WAVE_STARTED:
        case GameEvent::WAVE_COMPLETED:
            // Update UI elements as needed
            break;
        default:
            break;
    }
}

void GameView::render() {
    // Update camera target (center on player or follow player)
    camera.target = { (float)GetScreenWidth() / 2, (float)GetScreenHeight() / 2 };
    
    BeginMode2D(camera);
        renderBackground();
        renderGrid();
        renderTowers();
        renderEnemies();
        renderProjectiles();
    EndMode2D();
    
    renderUI();
}

void GameView::handleInput() {
    // Handle mouse input for tower placement, UI interaction, etc.
    Vector2 mousePos = GetMousePosition();
    hoveredCell = screenToWorld(mousePos);
    
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        // Handle left mouse click
        if (!isMouseOverUI()) {
            // Place tower or select tower
        }
    }
}

bool GameView::isMouseOverUI() const {
    Vector2 mousePos = GetMousePosition();
    return CheckCollisionPointRec(mousePos, sidebar) || 
           CheckCollisionPointRec(mousePos, buildMenu) ||
           CheckCollisionPointRec(mousePos, towerInfoPanel);
}

void GameView::renderBackground() {
    // Draw a simple background
    ClearBackground(DARKGREEN);
    
    // Draw a simple path (just a brown rectangle for now)
    DrawRectangle(0, GetScreenHeight() / 2 - 50, GetScreenWidth(), 100, BROWN);
}

void GameView::renderGrid() {
    // Draw a simple grid
    int cellSize = 40;
    
    // Draw vertical lines
    for (int x = 0; x < GetScreenWidth(); x += cellSize) {
        DrawLine(x, 0, x, GetScreenHeight(), ColorAlpha(LIGHTGRAY, 0.3f));
    }
    
    // Draw horizontal lines
    for (int y = 0; y < GetScreenHeight(); y += cellSize) {
        DrawLine(0, y, GetScreenWidth(), y, ColorAlpha(LIGHTGRAY, 0.3f));
    }
}

void GameView::renderTowers() {
    // Draw all active towers
    for (const auto& tower : model.getTowers()) {
        if (!tower->getIsActive()) continue;
        
        Vector2 pos = tower->getPosition();
        float size = 20.0f; // Base size for towers
        
        // Different colors and shapes for different tower types
        Color towerColor;
        switch (tower->getType()) {
            case TowerType::BASIC: 
                towerColor = BLUE;
                // Draw basic tower as a circle with a small circle on top
                DrawCircleV(pos, size, towerColor);
                DrawCircleV(pos, size * 0.6f, ColorBrightness(towerColor, 0.2f));
                break;
                
            case TowerType::SNIPER: 
                towerColor = DARKBLUE;
                // Draw sniper tower as a triangle pointing up
                DrawTriangle(
                    {pos.x, pos.y - size}, 
                    {pos.x - size, pos.y + size * 0.7f}, 
                    {pos.x + size, pos.y + size * 0.7f}, 
                    towerColor
                );
                break;
                
            case TowerType::SPLASH: 
                towerColor = ORANGE;
                // Draw splash tower as a square with an X
                DrawRectangleV(
                    {pos.x - size, pos.y - size}, 
                    {size * 2, size * 2}, 
                    towerColor
                );
                DrawLineEx(
                    {pos.x - size, pos.y - size}, 
                    {pos.x + size, pos.y + size}, 
                    3, 
                    YELLOW
                );
                DrawLineEx(
                    {pos.x + size, pos.y - size}, 
                    {pos.x - size, pos.y + size}, 
                    3, 
                    YELLOW
                );
                break;
                
            case TowerType::SLOW: 
                towerColor = SKYBLUE;
                // Draw slow tower as a hexagon
                DrawCircleV(pos, size, towerColor);
                for (int i = 0; i < 6; i++) {
                    float angle1 = i * PI / 3.0f;
                    float angle2 = (i + 1) * PI / 3.0f;
                    DrawLineEx(
                        {pos.x + cosf(angle1) * size, pos.y + sinf(angle1) * size},
                        {pos.x + cosf(angle2) * size, pos.y + sinf(angle2) * size},
                        3.0f,
                        BLUE
                    );
                }
                break;
                
            case TowerType::POISON: 
                towerColor = Color{128, 0, 128, 255}; // Purple
                // Draw poison tower as a diamond with dots
                DrawRectangleRotated(pos, size * 1.4f, size * 1.4f, 45.0f, towerColor);
                DrawCircleV(pos, size * 0.3f, LIME);
                break;
                
            default: 
                towerColor = GRAY;
                DrawCircleV(pos, size, towerColor);
        }
        
        // Draw tower level indicator
        int level = tower->getLevel();
        if (level > 1) {
            DrawText(
                TextFormat("%d", level), 
                pos.x - 4, 
                pos.y - 8, 
                12, 
                WHITE
            );
        }
        
        // Draw range when selected
        if (showTowerRange) {
            DrawCircleLines(pos.x, pos.y, tower->getRange(), ColorAlpha(towerColor, 0.3f));
        }
    }
}

void GameView::renderEnemies() {
    // Draw all active enemies
    for (const auto& enemy : model.getEnemies()) {
        if (!enemy->getIsActive()) continue;
        
        Vector2 pos = enemy->getPosition();
        float size = 15.0f; // Base size for enemies
        
        // Different shapes and colors for different enemy types
        switch (enemy->getType()) {
            case EnemyType::BASIC: {
                // Basic enemy - simple circle
                DrawCircleV(pos, size, RED);
                DrawCircleLines(pos.x, pos.y, size, DARKRED);
                break;
            }
            case EnemyType::FAST: {
                // Fast enemy - triangle pointing in movement direction
                float direction = atan2f(enemy->getVelocity().y, enemy->getVelocity().x);
                Vector2 p1 = {pos.x + cosf(direction) * size, pos.y + sinf(direction) * size};
                Vector2 p2 = {pos.x + cosf(direction + 2.5f) * size, pos.y + sinf(direction + 2.5f) * size};
                Vector2 p3 = {pos.x + cosf(direction - 2.5f) * size, pos.y + sinf(direction - 2.5f) * size};
                DrawTriangle(p1, p2, p3, ORANGE);
                break;
            }
            case EnemyType::TANK: {
                // Tank enemy - large square with armor
                float tankSize = size * 1.5f;
                DrawRectangleV(
                    {pos.x - tankSize/2, pos.y - tankSize/2}, 
                    {tankSize, tankSize}, 
                    DARKGRAY
                );
                // Armor plating
                DrawRectangleLinesEx(
                    {pos.x - tankSize/2, pos.y - tankSize/2, tankSize, tankSize}, 
                    3.0f, 
                    GRAY
                );
                DrawLineEx(
                    {pos.x - tankSize/2, pos.y - tankSize/2},
                    {pos.x + tankSize/2, pos.y + tankSize/2},
                    2.0f,
                    GRAY
                );
                DrawLineEx(
                    {pos.x + tankSize/2, pos.y - tankSize/2},
                    {pos.x - tankSize/2, pos.y + tankSize/2},
                    2.0f,
                    GRAY
                );
                break;
            }
            case EnemyType::FLYING: {
                // Flying enemy - diamond shape
                Vector2 points[4] = {
                    {pos.x, pos.y - size},
                    {pos.x + size, pos.y},
                    {pos.x, pos.y + size},
                    {pos.x - size, pos.y}
                };
                DrawTriangleStrip(points, 4, SKYBLUE);
                break;
            }
            case EnemyType::BOSS: {
                // Boss enemy - large with multiple segments
                float bossSize = size * 2.0f;
                
                // Main body
                DrawCircleV(pos, bossSize, PURPLE);
                
                // Decorative elements
                for (int i = 0; i < 8; i++) {
                    float angle = GetTime() * 2 + i * PI / 4.0f;
                    DrawCircle(
                        pos.x + cosf(angle) * bossSize * 0.7f,
                        pos.y + sinf(angle) * bossSize * 0.7f,
                        bossSize * 0.3f,
                        ColorAlpha(MAGENTA, 0.7f)
                    );
                }
                break;
            }
            default: {
                // Default enemy - circle
                DrawCircleV(pos, size, GRAY);
            }
        }
        
        // Draw health bar for all enemies
        float healthPercent = (float)enemy->getHealth() / enemy->getMaxHealth();
        float barWidth = size * 2;
        float barHeight = 4.0f;
        float barX = pos.x - size;
        float barY = pos.y - size - 8.0f;
        
        // Adjust bar position for larger enemies
        if (enemy->getType() == EnemyType::TANK || enemy->getType() == EnemyType::BOSS) {
            barY = pos.y - size * 1.5f - 12.0f;
            barWidth = size * 3;
            barX = pos.x - barWidth / 2;
        }
        
        // Background
        DrawRectangle(barX, barY, barWidth, barHeight, DARKGRAY);
        // Health
        DrawRectangle(
            barX, 
            barY, 
            barWidth * healthPercent, 
            barHeight, 
            healthPercent > 0.5f ? GREEN : 
            healthPercent > 0.2f ? YELLOW : RED
        );
        // Border
        DrawRectangleLines(barX, barY, barWidth, barHeight, BLACK);
    }
}

void GameView::renderProjectiles() {
    // Draw all active projectiles
    for (const auto& projectile : model.getProjectiles()) {
        if (!projectile->getIsActive()) continue;
        
        Vector2 pos = projectile->getPosition();
        float size = 5.0f;
        
        // Different projectiles based on tower type
        switch (projectile->getType()) {
            case TowerType::BASIC: {
                // Basic projectile - simple circle
                DrawCircleV(pos, size, YELLOW);
                break;
            }
            case TowerType::SNIPER: {
                // Sniper projectile - thin line
                Vector2 dir = Vector2Normalize(projectile->getVelocity());
                DrawLineEx(
                    {pos.x - dir.x * size * 2, pos.y - dir.y * size * 2},
                    {pos.x + dir.x * size * 2, pos.y + dir.y * size * 2},
                    2.0f,
                    RED
                );
                break;
            }
            case TowerType::SPLASH: {
                // Splash projectile - circle with lines
                DrawCircleV(pos, size * 1.5f, ORANGE);
                for (int i = 0; i < 4; i++) {
                    float angle = GetTime() * 10 + i * PI / 2.0f;
                    DrawLineEx(
                        pos,
                        {pos.x + cosf(angle) * size * 1.5f, pos.y + sinf(angle) * size * 1.5f},
                        2.0f,
                        YELLOW
                    );
                }
                break;
            }
            case TowerType::SLOW: {
                // Slow projectile - snowflake shape
                DrawCircleV(pos, size, SKYBLUE);
                for (int i = 0; i < 6; i++) {
                    float angle = i * PI / 3.0f;
                    DrawLineEx(
                        pos,
                        {pos.x + cosf(angle) * size, pos.y + sinf(angle) * size},
                        1.5f,
                        BLUE
                    );
                }
                break;
            }
            case TowerType::POISON: {
                // Poison projectile - green circle with a plus
                DrawCircleV(pos, size, LIME);
                DrawLineEx(
                    {pos.x - size, pos.y},
                    {pos.x + size, pos.y},
                    2.0f,
                    DARKGREEN
                );
                DrawLineEx(
                    {pos.x, pos.y - size},
                    {pos.x, pos.y + size},
                    2.0f,
                    DARKGREEN
                );
                break;
            }
            default: {
                // Default projectile
                DrawCircleV(pos, size, WHITE);
            }
        }
    }
}

void GameView::renderUI() {
    // Draw sidebar
    DrawRectangleRec(sidebar, ColorAlpha(DARKGRAY, 0.8f));
    
    // Draw game info
    int yPos = 20;
    DrawTextEx(defaultFont, TextFormat("Wave: %d/%d", 
        model.getWaveManager().getCurrentWave() + 1, 
        model.getWaveManager().getTotalWaves()), 
        { sidebar.x + 10, (float)yPos }, 20, 2, WHITE);
    yPos += 30;
    
    DrawTextEx(defaultFont, TextFormat("Health: %d", model.getPlayerHealth()), 
        { sidebar.x + 10, (float)yPos }, 20, 2, WHITE);
    yPos += 30;
    
    DrawTextEx(defaultFont, TextFormat("Gold: %d", model.getPlayerGold()), 
        { sidebar.x + 10, (float)yPos }, 20, 2, GOLD);
    yPos += 30;
    
    DrawTextEx(defaultFont, TextFormat("Score: %d", model.getScore()), 
        { sidebar.x + 10, (float)yPos }, 20, 2, WHITE);
    yPos += 50;
    
    // Draw build menu
    DrawRectangleRec(buildMenu, ColorAlpha(BLACK, 0.7f));
    DrawTextEx(defaultFont, "BUILD", { buildMenu.x + 10, buildMenu.y + 10 }, 20, 2, WHITE);
    
    // Draw tower build buttons
    float buttonY = buildMenu.y + 40;
    float buttonWidth = buildMenu.width - 20;
    float buttonHeight = 40;
    
    // Basic Tower
    if (drawButton("Basic Tower", { buildMenu.x + 10, buttonY, buttonWidth, buttonHeight }, 
                  BLUE, model.getPlayerGold() >= 100)) {
        selectedTowerType = TowerType::BASIC;
    }
    buttonY += buttonHeight + 5;
    
    // Sniper Tower
    if (drawButton("Sniper", { buildMenu.x + 10, buttonY, buttonWidth, buttonHeight }, 
                  RED, model.getPlayerGold() >= 200)) {
        selectedTowerType = TowerType::SNIPER;
    }
    buttonY += buttonHeight + 5;
    
    // Splash Tower
    if (drawButton("Splash", { buildMenu.x + 10, buttonY, buttonWidth, buttonHeight }, 
                  YELLOW, model.getPlayerGold() >= 150)) {
        selectedTowerType = TowerType::SPLASH;
    }
    buttonY += buttonHeight + 5;
    
    // Slow Tower
    if (drawButton("Slow", { buildMenu.x + 10, buttonY, buttonWidth, buttonHeight }, 
                  SKYBLUE, model.getPlayerGold() >= 175)) {
        selectedTowerType = TowerType::SLOW;
    }
    buttonY += buttonHeight + 5;
    
    // Poison Tower
    if (drawButton("Poison", { buildMenu.x + 10, buttonY, buttonWidth, buttonHeight }, 
                  PURPLE, model.getPlayerGold() >= 250)) {
        selectedTowerType = TowerType::POISON;
    }
    
    // Draw tower info panel if a tower is selected
    // (Implementation needed)
}

bool GameView::drawButton(const char* text, Rectangle bounds, Color color, bool enabled) {
    Color buttonColor = enabled ? color : GRAY;
    if (enabled && CheckCollisionPointRec(GetMousePosition(), bounds)) {
        buttonColor = ColorBrightness(buttonColor, 0.2f);
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            return true;
        }
    }
    
    DrawRectangleRec(bounds, buttonColor);
    DrawRectangleLinesEx(bounds, 2, ColorBrightness(buttonColor, -0.2f));
    
    // Center text in button
    Vector2 textSize = MeasureTextEx(defaultFont, text, 16, 1);
    Vector2 textPos = {
        bounds.x + (bounds.width - textSize.x) / 2,
        bounds.y + (bounds.height - textSize.y) / 2
    };
    
    DrawTextEx(defaultFont, text, textPos, 16, 1, enabled ? WHITE : LIGHTGRAY);
    
    return false;
}

void GameView::renderHealthBar(float x, float y, float width, float height, float value, float maxValue, Color color) {
    // Draw background
    DrawRectangle(x, y, width, height, ColorAlpha(BLACK, 0.5f));
    
    // Draw health
    float healthWidth = (value / maxValue) * width;
    DrawRectangle(x, y, healthWidth, height, color);
    
    // Draw border
    DrawRectangleLinesEx({ x, y, width, height }, 1, BLACK);
}

Vector2 GameView::screenToWorld(Vector2 screenPos) const {
    return GetScreenToWorld2D(screenPos, camera);
}

Vector2 GameView::worldToGrid(Vector2 worldPos) const {
    int cellSize = 40; // Should match the grid cell size
    return { floorf(worldPos.x / cellSize), floorf(worldPos.y / cellSize) };
}

Vector2 GameView::gridToWorld(Vector2 gridPos) const {
    int cellSize = 40; // Should match the grid cell size
    return { gridPos.x * cellSize + cellSize / 2, gridPos.y * cellSize + cellSize / 2 };
}

Rectangle GameView::getCellRect(Vector2 gridPos) const {
    int cellSize = 40; // Should match the grid cell size
    return { gridPos.x * cellSize, gridPos.y * cellSize, (float)cellSize, (float)cellSize };
}
