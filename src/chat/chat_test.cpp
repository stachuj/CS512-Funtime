#include "raylib.h"
#include "chat.hpp"

int main() {
    InitWindow(800, 600, "Local Chat Test");
    SetTargetFPS(60);

    Chat chat;

    while(!WindowShouldClose()) {
        chat.Update();

        BeginDrawing();
        ClearBackground(BLACK);
        chat.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
