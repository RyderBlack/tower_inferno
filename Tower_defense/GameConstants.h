#pragma once

// Game constants
const int WINDOW_WIDTH = 1600;  // Fixed window width
const int WINDOW_HEIGHT = 800;  // Fixed window height
const int UI_WIDTH = 300;       // Width of the UI panel
const int TILE_SIZE = 72;       // Increased tile size to 64x64 pixels
const int GRID_WIDTH = 18;      // Number of horizontal tiles
const int GRID_HEIGHT = 11;     // Number of vertical tiles
const int GRID_START_X = (WINDOW_WIDTH - UI_WIDTH - (GRID_WIDTH * TILE_SIZE)) / 2;
const int GRID_START_Y = (WINDOW_HEIGHT - (GRID_HEIGHT * TILE_SIZE)) / 2; 