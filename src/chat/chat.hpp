#pragma once
#include "raylib.h"
#include <string>
#include <vector>

class Chat {
public:
    Chat();

    void Update();
    void Draw();

private:
    std::vector<std::string> messages;
    std::string currentInput;

    void AddMessage(const std::string &msg);
};
