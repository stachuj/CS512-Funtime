#include "chat.hpp"
#include <iostream>

Chat::Chat() {}

void Chat::AddMessage(const std::string &msg) {
    messages.push_back(msg);
}


void Chat::Update() {
    //Handles typing letters/symbols
    int key = GetCharPressed();
    while (key > 0) {
        if (key >= 32 && key <= 125) { //Any normal characters
            currentInput.push_back((char)key);
        }
        key = GetCharPressed();
    }

    //Backspace (one character at a time)
    if (IsKeyPressed(KEY_BACKSPACE) && !currentInput.empty()) {
        currentInput.pop_back();
    }

    //Enter key adds message
    if ((IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_KP_ENTER)) && !currentInput.empty()) {
        AddMessage(currentInput);  //add typed message
        currentInput.clear();      //clear input line
    }
}
void Chat::Draw() {
    int visibleCount = 6; //Limits to 6 messages
    int total = messages.size();
    int startIndex = (total > visibleCount) ? total - visibleCount : 0;

    int yStart = 300; //Starting position for mesages
    int lineHeight = 25;

    //Draw messages from top to bottom
    for (int i = startIndex; i < total; i++) {
        DrawText(messages[i].c_str(), 10, yStart, 20, RAYWHITE);
        yStart += lineHeight;
    }

    //Add blinking cursor
    std::string displayInput = currentInput;

    //Blink every half second
    if (((int)(GetTime() * 2)) % 2 == 0) {
        displayInput += "_";  
    }

    DrawText(displayInput.c_str(), 10, yStart + 20, 20, GREEN);
}