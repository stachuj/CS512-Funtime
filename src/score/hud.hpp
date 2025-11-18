#pragma once
#include "raylib.h"
#include "game_state.hpp"

inline void DrawHUD(const GameState& gs) {
    DrawText(TextFormat("SCORE: %d", gs.score), 10+2, 10+2, 28, BLACK);
    DrawText(TextFormat("SCORE: %d", gs.score), 10, 10, 28, WHITE);
    const int fontSize = 28;
    int seconds = (int)ceilf(gs.timeRemaining);
    const char* t = TextFormat("TIME: %02d", seconds);
    int w = MeasureText(t, fontSize);
    DrawText(t, GetScreenWidth()/2 - w/2 + 2, 10+2, fontSize, BLACK);
    DrawText(t, GetScreenWidth()/2 - w/2, 10, fontSize, WHITE);
    if (gs.timeUp) {
        const char* msg = "TIME'S UP! PRESS R TO RESTART";
        int mw = MeasureText(msg, 36);
        DrawText(msg, GetScreenWidth()/2 - mw/2, GetScreenHeight()/2 - 18, 36, YELLOW);
    }
}
