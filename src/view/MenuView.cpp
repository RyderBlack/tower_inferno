#include "MenuView.h"

void MenuView::Render(bool isSoundOn) {
    DrawText("TOWER INFERNO", 440, 200, 48, ORANGE);
    DrawText("ENTER: Start", 540, 300, 24, DARKGRAY);
    DrawText(TextFormat("S: Toggle Sound (%s)", isSoundOn ? "ON" : "OFF"), 520, 340, 24, DARKGRAY);
    DrawText("Q: Quit", 570, 380, 24, DARKGRAY);
}
