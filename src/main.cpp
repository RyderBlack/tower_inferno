#include "controller/GameController.h"

int main() {
    GameController game;
    game.Initialize();
    game.Run();
    game.Shutdown();
    return 0;
}