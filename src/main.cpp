#include "game_object.hpp"
#include "test_object.hpp"
#include "tilemap/tilemap.hpp"
#include "chat/chat.hpp"
#include "character.hpp"
#include <vector>
#include <raylib.h>

using namespace std;

Chat chat;

int main() {
    std::vector<GameObject *> objects;

    initializeTilemap();
    getTilemap("testfile.txt");

    InitAudioDevice();

    Sound scream = LoadSound("assets/scream.wav");
    Sound pew = LoadSound("assets/pew.wav");
    Sound mew = LoadSound("assets/mew.wav");

    InitWindow(800, 600, "CS512 Funtime");
    SetTargetFPS(60);

    objects.push_back(new TestObject({100.0, 400.0}));
    Character* player = new Character({400, 300}, "assets");
    objects.push_back(player);

    PlaySound(scream);

    // ✅ Single main loop
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_UP)) PlaySound(scream);
        if (IsKeyPressed(KEY_DOWN)) PlaySound(pew);
        if (IsKeyPressed(KEY_LEFT)) PlaySound(mew);

        float deltaTime = GetFrameTime();
        for (auto object : objects)
            object->Update(deltaTime);

        chat.Update();

        Vector2 mousePosition = GetMousePosition();
        //bool mouseOnWall = isWall(mousePosition.x, mousePosition.y);

        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            displayTilemap();

            for (auto object : objects)
                object->Draw();

            chat.Draw();

            DrawText("Press UP for Scream", 280, 150, 20, RED);
            DrawText("Press DOWN for Pew", 280, 200, 20, BLUE);
            DrawText("Press LEFT for Mew", 280, 250, 20, DARKGREEN);

            // ✅ Dark color so it's visible
            DrawText("This is Jeremy the purple square.", 10, 10, 20, BLACK);
        }
        EndDrawing();
    }

    // ✅ Cleanup outside the loop
    for (auto object : objects)
        delete object;

    UnloadSound(scream);
    UnloadSound(pew);
    UnloadSound(mew);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}
