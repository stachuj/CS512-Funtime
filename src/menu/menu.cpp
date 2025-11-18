#include "menu.hpp"

Menu::Menu(MenuType menuType) : selected(0), type(menuType), volume(0.5f) {}  // start at 50%

MenuResult Menu::Update() {
    int optionCount = 0;

    if (type == MenuType::Main)        optionCount = 5;
    else if (type == MenuType::Pause)  optionCount = 4;
    else if (type == MenuType::Settings) optionCount = 2; // Volume + Back

    // Keyboard navigation
    if (IsKeyPressed(KEY_UP)) selected = (selected - 1 + optionCount) % optionCount;
    if (IsKeyPressed(KEY_DOWN)) selected = (selected + 1) % optionCount;

    // Adjust volume 
    if (type == MenuType::Settings && selected == 0) {
        if (IsKeyPressed(KEY_RIGHT)) {
            volume += 0.1f;
            if (volume > 1.0f) volume = 1.0f;
            std::cout << "Volume: " << (int)(volume * 100) << "%" << std::endl;
        }
        if (IsKeyPressed(KEY_LEFT)) {
            volume -= 0.1f;
            if (volume < 0.0f) volume = 0.0f;
            std::cout << "Volume: " << (int)(volume * 100) << "%" << std::endl;
        }
    }

    // Enter key actions
    if (IsKeyPressed(KEY_ENTER)) {
        if (type == MenuType::Main) {
            if (selected == 0) return MenuResult::StartGame;
            if (selected == 1) return MenuResult::Rules;
            if (selected == 2) return MenuResult::StartEditor;
            if (selected == 3) return MenuResult::Settings;
            if (selected == 4) return MenuResult::Exit;
        } 
        else if (type == MenuType::Pause) {
            if (selected == 0) return MenuResult::Resume;
            if (selected == 1) return MenuResult::Rules;
            if (selected == 2) return MenuResult::StartEditor;
            if (selected == 3) return MenuResult::Exit;
        } 
        else if (type == MenuType::Settings) {
            if (selected == 1) { // Back
                type = MenuType::Main;
                selected = 0;
                return MenuResult::None;
            }
        }
    }

    return MenuResult::None;
}

void Menu::Draw() {
    int xPos = 400;
    ClearBackground(BLACK);

    // Header
    if (type == MenuType::Main)
        DrawText("CS512 Funtime", 310, 150, 60, RAYWHITE);
    else if (type == MenuType::Pause)
        DrawText("Paused", 380, 150, 60, RAYWHITE);
    else if (type == MenuType::Settings)
        DrawText("Settings", 360, 150, 60, RAYWHITE);

    // Main menu
    if (type == MenuType::Main) {
        DrawText("Start Game",   xPos, 280, 30, selected == 0 ? GREEN : GRAY);
        DrawText("Game Rules",   xPos, 330, 30, selected == 1 ? GREEN : GRAY);
        DrawText("Level Editor", xPos, 380, 30, selected == 2 ? GREEN : GRAY);
        DrawText("Settings",     xPos, 430, 30, selected == 3 ? GREEN : GRAY);
        DrawText("Quit",         xPos, 480, 30, selected == 4 ? GREEN : GRAY);
    }

    // Pause menu
    else if (type == MenuType::Pause) {
        DrawText("Resume Game",  xPos, 280, 30, selected == 0 ? GREEN : GRAY);
        DrawText("Game Rules",   xPos, 330, 30, selected == 1 ? GREEN : GRAY);
        DrawText("Level Editor", xPos, 380, 30, selected == 2 ? GREEN : GRAY);
        DrawText("Quit",         xPos, 430, 30, selected == 3 ? GREEN : GRAY);
    }

    // Settings menu
    else if (type == MenuType::Settings) {
        // Volume text
        std::string volText = "Volume: " + std::to_string((int)(volume * 100)) + "%";
        DrawText(volText.c_str(), xPos, 280, 30, selected == 0 ? GREEN : GRAY);

        // Volume bar
        int barWidth = 200;
        int filled = (int)(volume * barWidth);
        DrawRectangle(xPos, 320, barWidth, 20, GRAY);
        DrawRectangle(xPos, 320, filled, 20, GREEN);

        // Back option
        DrawText("Back", xPos, 360, 30, selected == 1 ? GREEN : GRAY);
    }
}
