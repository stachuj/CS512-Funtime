#pragma once
#include "raylib.h"

enum class MenuResult {
    None,
    StartGame,
    StartEditor,
    Rules,
    Exit,
    Resume
};

enum class GameStates {
    Menu,
    LevelSelect,
    Game,
    Editor,
    Pause,
    Rules,
    Exit
};

enum class MenuType {
    Main,
    Pause
};

class Menu {
private:
    int selected;
    MenuType type;

public:
    Menu(MenuType menuType = MenuType::Main);
    MenuResult Update();
    void Draw();
    MenuType GetType() const { return type; }
};
