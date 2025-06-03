#include "GameController.h"
#include "raylib.h"

GameController::GameController() {
    // Initialize camera
    camera = {
        .position = {10.0f, 12.0f, 10.0f},
        .target = {0.0f, 0.0f, 0.0f},
        .up = {0.0f, 1.0f, 0.0f},
        .fovy = 45.0f,
        .projection = CAMERA_PERSPECTIVE
    };
}

GameController::~GameController() {
    Shutdown();
}

void GameController::Initialize() {
    InitWindow(1280, 720, "Tower Inferno - 3D Tower Defense");
    InitAudioDevice();
    SetTargetFPS(60);
    
    // Load resources
    music = LoadMusicStream("assets/Tower_inferno_instru.mp3");
    PlayMusicStream(music);
    
    // Initialize views
    gameView.Initialize();
}

void GameController::Run() {
    while (!WindowShouldClose()) {
        ProcessInput();
        Update(GetFrameTime());
        Render();
    }
}

void GameController::Shutdown() {
    StopMusicStream(music);
    UnloadMusicStream(music);
    CloseAudioDevice();
    CloseWindow();
}

void GameController::ProcessInput() {
    switch (game.GetState()) {
        case Game::State::MENU:
            HandleMenuInput();
            break;
        case Game::State::PLAYING:
            HandleGameInput();
            break;
        case Game::State::GAME_OVER:
            HandleGameOverInput();
            break;
    }
}

void GameController::Update(float deltaTime) {
    UpdateMusicStream(music);
    SetMusicVolume(music, game.IsSoundOn() ? 1.0f : 0.0f);
    game.Update(deltaTime);
}

void GameController::Render() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    
    switch (game.GetState()) {
        case Game::State::MENU:
            menuView.Render(game.IsSoundOn());
            break;
        case Game::State::PLAYING:
            gameView.Render(game, camera);
            gameView.RenderUI(game);
            break;
        case Game::State::GAME_OVER:
            gameView.Render(game, camera);
            gameView.RenderUI(game);
            break;
    }
    
    EndDrawing();
}

void GameController::HandleMenuInput() {
    if (IsKeyPressed(KEY_ENTER)) {
        game.StartGame();
    } else if (IsKeyPressed(KEY_S)) {
        game.ToggleSound();
    } else if (IsKeyPressed(KEY_Q)) {
        CloseWindow();
    }
}

void GameController::HandleGameInput() {
    if (IsKeyPressed(KEY_ESCAPE)) {
        // Pause or return to menu
    }
    // Add more game-specific input handling here
}

void GameController::HandleGameOverInput() {
    if (IsKeyPressed(KEY_ENTER)) {
        game.Reset();
    }
}
