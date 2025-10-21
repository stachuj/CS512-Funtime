#pragma once
#include "raylib.h"

enum class MenuResult {
    None,
    StartGame,
    Rules,
    Exit,
    Resume
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
