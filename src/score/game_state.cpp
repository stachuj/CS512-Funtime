#include "game_state.hpp"

void GameState::UpdateTimer(float deltaTime) {
    // ADDED: countdown timer
    if (!this->timeUp) {
        this->timeRemaining -= deltaTime;
        if (this->timeRemaining <= 0.0f) {
            this->timeRemaining = 0.0f;
            this->timeUp = true;
        }
    }
}

void GameState::ResetTimer() {
    timeRemaining = timeLimit;
    timeUp = false;
}