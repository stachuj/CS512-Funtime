#include "raylib.h"
#include "game_object.hpp"
#include "character.hpp"
#include <vector>

int main() {
    InitWindow(800, 600, "Animated Character Demo");
    SetTargetFPS(60);

    std::vector<GameObject*> objects;

    Character* player = new Character({400, 300}, "assets");
    objects.push_back(player);

    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        for (auto obj : objects)
            obj->Update(deltaTime);

        BeginDrawing();
        ClearBackground(RAYWHITE);

        for (auto obj : objects)
            obj->Draw();

        EndDrawing();
    }

    for (auto obj : objects)
        delete obj;

    CloseWindow();
    return 0;
}
