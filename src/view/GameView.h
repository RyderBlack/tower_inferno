#ifndef TOWER_INFERNO_GAMEVIEW_H
#define TOWER_INFERNO_GAMEVIEW_H

#include <raylib.h>
#include "../model/GameModel.h"
#include "../utils/Observable.h"

class GameView : public Observer {
private:
    const GameModel& model;
    Camera2D camera;
    
    // UI elements
    Rectangle sidebar;
    Rectangle buildMenu;
    Rectangle towerInfoPanel;
    
    // Textures
    Texture2D background;
    Texture2D towerIcons[5]; // For different tower types
    Texture2D enemyTextures[5]; // For different enemy types
    Texture2D projectileTexture;
    
    // Fonts
    Font defaultFont;
    Font titleFont;
    
    // Game state
    bool showTowerRange;
    TowerType selectedTowerType;
    Vector2 hoveredCell;
    
public:
    explicit GameView(const GameModel& model);
    ~GameView() override;
    
    // Observer pattern
    void onNotify(int eventType, void* subject) override;
    
    // Main rendering methods
    void render();
    
    // Input handling
    void handleInput();
    
    // Getters for UI state
    bool isMouseOverUI() const;
    Vector2 getHoveredCell() const { return hoveredCell; }
    TowerType getSelectedTowerType() const { return selectedTowerType; }
    
private:
    // Rendering methods
    void renderBackground();
    void renderGrid();
    void renderTowers();
    void renderEnemies();
    void renderProjectiles();
    void renderUI();
    void renderSidebar();
    void renderBuildMenu();
    void renderTowerInfo();
    void renderGameOver();
    void renderWaveInfo();
    void renderHealthBar(float x, float y, float width, float height, float value, float maxValue, Color color);
    
    // Helper methods
    Vector2 screenToWorld(Vector2 screenPos) const;
    Vector2 worldToGrid(Vector2 worldPos) const;
    Vector2 gridToWorld(Vector2 gridPos) const;
    Rectangle getCellRect(Vector2 gridPos) const;
    
    // UI helpers
    void drawButton(const char* text, Rectangle bounds, Color color, Color textColor = WHITE);
    bool isButtonPressed(Rectangle bounds) const;
    bool isButtonHovered(Rectangle bounds) const;
};

#endif //TOWER_INFERNO_GAMEVIEW_H
