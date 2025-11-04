#include "menu.hpp"

Menu::Menu(MenuType menuType) : selected(0), type(menuType) {}

MenuResult Menu::Update() {

    int optionCount = 4;

    if (IsKeyPressed(KEY_UP)) selected = (selected - 1 + optionCount) % optionCount;
    if (IsKeyPressed(KEY_DOWN)) selected = (selected + 1) % optionCount;

    if (IsKeyPressed(KEY_ENTER)) {
        if (type == MenuType::Main) {
            if (selected == 0) return MenuResult::StartGame;
            if (selected == 1) return MenuResult::Rules;
            if (selected == 2) return MenuResult::StartEditor;
            if (selected == 3) return MenuResult::Exit;
        } else {
            if (selected == 0) return MenuResult::Resume;
            if (selected == 1) return MenuResult::Rules;
            if (selected == 2) return MenuResult::StartEditor;
            if (selected == 3) return MenuResult::Exit;
        }
    }

    Vector2 mouse = GetMousePosition();
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        if (type == MenuType::Main) {
            if (CheckCollisionPointRec(mouse, {300, 280, 200, 40})) return MenuResult::StartGame;
            if (CheckCollisionPointRec(mouse, {300, 330, 200, 40})) return MenuResult::Rules;
            if (CheckCollisionPointRec(mouse, {300, 380, 200, 40})) return MenuResult::StartEditor;
            if (CheckCollisionPointRec(mouse, {300, 430, 200, 40})) return MenuResult::Exit;
        } else {
            if (CheckCollisionPointRec(mouse, {300, 280, 200, 40})) return MenuResult::Resume;
            if (CheckCollisionPointRec(mouse, {300, 330, 200, 40})) return MenuResult::Rules;
            if (CheckCollisionPointRec(mouse, {300, 380, 200, 40})) return MenuResult::StartEditor;
            if (CheckCollisionPointRec(mouse, {300, 430, 200, 40})) return MenuResult::Exit;
        }
    }

    return MenuResult::None;
}

void Menu::Draw() {
    ClearBackground(BLACK);
    if (type == MenuType::Main)
        DrawText("CS512 Funtime", 240, 150, 40, RAYWHITE);
    else
        DrawText("Paused", 330, 150, 40, RAYWHITE);

    if (type == MenuType::Main) {
        DrawText("Start Game", 300, 280, 30, selected == 0 ? GREEN : GRAY);
        DrawText("Game Rules", 300, 330, 30, selected == 1 ? GREEN : GRAY);
        DrawText("Level Editor", 300, 380, 30, selected == 2 ? GREEN : GRAY);
        DrawText("Quit",       300, 430, 30, selected == 3 ? GREEN : GRAY);
    } else {
        DrawText("Resume Game", 300, 280, 30, selected == 0 ? GREEN : GRAY);
        DrawText("Game Rules",  300, 330, 30, selected == 1 ? GREEN : GRAY);
        DrawText("Level Editor", 300, 380, 30, selected == 2 ? GREEN : GRAY);
        DrawText("Quit",       300, 430, 30, selected == 3 ? GREEN : GRAY);
    }
}
