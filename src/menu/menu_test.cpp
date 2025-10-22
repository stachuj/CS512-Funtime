#include "raylib.h"
#include "menu.hpp"

enum class GameState {
    Menu,
    Game,
    Pause,
    Rules,
    Exit
};

/*
int main() {
    InitWindow(800, 600, "Menu + Pause Button Example");
    SetTargetFPS(60);

    GameState currentState = GameState::Menu;
    Menu mainMenu(MenuType::Main);
    Menu pauseMenu(MenuType::Pause);
    MenuType lastMenuType = MenuType::Main;  // remembers where rules came from

    while (!WindowShouldClose() && currentState != GameState::Exit) {
        BeginDrawing();

        switch (currentState) {
            case GameState::Menu: {
                MenuResult result = mainMenu.Update();
                mainMenu.Draw();

                if (result == MenuResult::StartGame)
                    currentState = GameState::Game;
                else if (result == MenuResult::Rules) {
                    currentState = GameState::Rules;
                    lastMenuType = MenuType::Main;
                } else if (result == MenuResult::Exit)
                    currentState = GameState::Exit;
            } break;

            case GameState::Game: {
                ClearBackground(DARKGREEN);
                DrawText("GAME RUNNING", 260, 250, 40, WHITE);

                // Pause button bottom-right
                Rectangle pauseBtn = {700, 550, 80, 30};
                DrawRectangleRec(pauseBtn, DARKGRAY);
                DrawText("Pause", pauseBtn.x + 10, pauseBtn.y + 5, 20, WHITE);

                // If clicked -> go to pause menu
                if (CheckCollisionPointRec(GetMousePosition(), pauseBtn) &&
                    IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    currentState = GameState::Pause;
                }
            } break;

            case GameState::Pause: {
                MenuResult result = pauseMenu.Update();
                pauseMenu.Draw();

                if (result == MenuResult::Resume)
                    currentState = GameState::Game;
                else if (result == MenuResult::Rules) {
                    currentState = GameState::Rules;
                    lastMenuType = MenuType::Pause;
                } else if (result == MenuResult::Exit)
                    currentState = GameState::Exit;
            } break;

            case GameState::Rules: {
                ClearBackground(DARKBLUE);
                DrawText("GAME RULES", 300, 120, 40, RAYWHITE);
                DrawText("1. Rule 1", 240, 200, 25, RAYWHITE);
                DrawText("2. Rule 2", 260, 240, 25, RAYWHITE);
                DrawText("3. Rule 3", 280, 280, 25, RAYWHITE);

                Rectangle returnBtn = {320, 360, 160, 40};
                DrawRectangleRec(returnBtn, DARKGRAY);
                DrawText("Return", returnBtn.x + 30, returnBtn.y + 10, 25, WHITE);

                if (CheckCollisionPointRec(GetMousePosition(), returnBtn) &&
                    IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    // Return to previous menu
                    currentState = (lastMenuType == MenuType::Pause)
                        ? GameState::Pause
                        : GameState::Menu;
                }
            } break;

            default: break;
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
*/