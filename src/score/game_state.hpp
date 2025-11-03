#pragma once
class GameState {
    public:
        int score = 0;
        float timeLimit = 60.0f;
        float timeRemaining = timeLimit;
        bool timeUp = false;
        int pointsPerCollectible = 10;

        void UpdateTimer(float deltaTime);
};