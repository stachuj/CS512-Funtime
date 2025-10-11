#include "raylib.h"
/*
int main() {
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Raylib Sound Test - Deepthi");

    InitAudioDevice();

    Sound scream = LoadSound("assets/scream.wav");
    Sound pew = LoadSound("assets/pew.wav");
    Sound mew = LoadSound("assets/mew.wav");

    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ONE)) PlaySound(scream);
        if (IsKeyPressed(KEY_TWO)) PlaySound(pew);
        if (IsKeyPressed(KEY_THREE)) PlaySound(mew);

        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Press 1 for Scream", 280, 150, 20, RED);
        DrawText("Press 2 for Pew", 310, 200, 20, BLUE);
        DrawText("Press 3 for Mew", 310, 250, 20, DARKGREEN);
        EndDrawing();
    }

    UnloadSound(scream);
    UnloadSound(pew);
    UnloadSound(mew);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
*/