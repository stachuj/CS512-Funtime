#include "raylib.h"
#include "menu.hpp"
#include <iostream>

int main() {
    const int screenWidth = 800;
    const int screenHeight = 600;
    InitWindow(screenWidth, screenHeight, "Menu Test");
    SetTargetFPS(60);

    Menu menu(MenuType::Main);

    while (!WindowShouldClose()) {
        MenuResult result = menu.Update();

        // Debug output
        if (result != MenuResult::None) {
            std::cout << "MenuResult: " << static_cast<int>(result) << std::endl;
        }

        // Handle menu results
        if (result == MenuResult::Exit)
            break;

        if (result == MenuResult::Settings) {
            menu = Menu(MenuType::Settings);
        }

        BeginDrawing();
        menu.Draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
