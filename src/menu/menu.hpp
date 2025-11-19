#pragma once
#include "raylib.h"
#include <iostream>
#include <string>

enum class MenuResult {
    None,
    StartGame,
    StartEditor,
    Rules,
    Settings,
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
    Settings,
    LevelBeat,
    Exit
};

enum class MenuType {
    Main,
    Pause,
    Settings
};

class Menu {
private:
    int selected;
    MenuType type;
    float volume;  

public:
    Menu(MenuType menuType = MenuType::Main);

    MenuResult Update();
    void Draw();

    MenuType GetType() const { return type; }
    float GetVolume() const { return volume; }  // For later integration
};
