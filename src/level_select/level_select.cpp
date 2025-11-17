#include "level_select.hpp"
#include <string>

int LevelSelect::Update() {

    int optionCount = 3 ;
    int levelCount = 30 ;

    if (IsKeyPressed(KEY_UP)) selected = (selected - 1 + optionCount) % optionCount;
    if (IsKeyPressed(KEY_DOWN)) selected = (selected + 1) % optionCount;

    switch(selected) {
        case 0: {
            if (IsKeyPressed(KEY_LEFT)) level = (level - 1 + levelCount) % levelCount ;
            if (IsKeyPressed(KEY_RIGHT)) level = (level + 1) % levelCount ;
        }break;
        case 1: {
            if (IsKeyPressed(KEY_ENTER)) return 1 ;
        }break;
        case 2: {
            if (IsKeyPressed(KEY_ENTER)) return 2 ;
        }break;
    }
    return 0 ;
}

void LevelSelect::Draw() {
    int xPos = 400;

    ClearBackground(BLACK);

    DrawText("Level Select", 310, 150, 60, RAYWHITE);

    DrawText(("Level: " + std::to_string(level + 1)).c_str(), xPos, 280, 30, selected == 0 ? GREEN : GRAY);
    DrawText("Start", xPos, 330, 30, selected == 1 ? GREEN : GRAY);
    DrawText("Return", xPos, 380, 30, selected == 2 ? GREEN : GRAY);
}

int LevelSelect::GetLevel() {
    return level ;
}