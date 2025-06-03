#pragma once
#include "raylib.h"
#include "../model/Game.h"

class GameView {
public:
    void Initialize();
    void Shutdown();
    void Render(const Game& game, const Camera& camera);
    void RenderUI(const Game& game);
    
private:
    void RenderEnemy(const class Enemy& enemy);
    void RenderTower(const class Tower& tower);
    void RenderHealthBar(Vector3 position, float healthRatio);
};
