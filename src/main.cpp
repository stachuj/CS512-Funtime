#include "game_object.hpp"
#include "test_object.hpp"
#include "tilemap.hpp"
#include "chat.hpp"
#include "character.hpp"
#include "test_object_A_Star.hpp"

#include "raylib-cpp.hpp"
#include <vector>

#include "game_state.hpp"     
#include "hud.hpp"           
#include "collectible.hpp"  
#include <string>

#define WINDOW_WIDTH 1024;
#define WINDOW_HEIGHT 768;

using namespace std;

GameState gameState;                   // holds score & timer
Character* playerPtr;
vector<GameObject *> enemies;
vector<Collectible> collectibles;      // pickups to score

int main() {

    initializeTilemap();
	//initializeAStarTestTilemap() ;

    InitAudioDevice();
    Sound scream = LoadSound("../../assets/scream.wav");
    Sound pew = LoadSound("../../assets/pew.wav");
    Sound mew = LoadSound("../../assets/mew.wav");
    Sound pickupSfx = pew;

    InitWindow(1024, 768, "CS512 Funtime");
    SetTargetFPS(60);

    playerPtr = Character::GetPlayer();

    enemies.push_back(new TestObjectAStar({80.0, 80.0}));

    Collectibles::SpawnRandom(collectibles, 10, {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()});

    //Character::GetPlayer()->SetPosition({400.0, 400.0});

    while (!WindowShouldClose()) {

        // This is the real time in seconds since the last update
        float deltaTime = GetFrameTime();

        gameState.UpdateTimer(deltaTime);

        playerPtr->Update(deltaTime);

        for (auto enemy: enemies)
            enemy->Update(deltaTime);

        Rectangle playerBox = playerPtr->GetCollisionBox();

        if (!gameState.timeUp) {
            int newlyPicked = Collectibles::Update(collectibles, &playerBox, pickupSfx);
            gameState.score += newlyPicked * gameState.pointsPerCollectible;
        }

        /*
        // Optional restart after time up
        if (gs.timeUp && IsKeyPressed(KEY_R)) {
            gs.score = 0;
            gs.timeRemaining = gs.timeLimit;
            gs.timeUp = false;
            for (auto& c : collectibles) c.active = true; // reactivate all
        }
        */
        
        BeginDrawing();

        ClearBackground(RAYWHITE);

        displayTilemap();

        playerPtr->Draw();

        for (auto enemy: enemies)
            enemy->Draw();

        Collectibles::Draw(collectibles);

        DrawHUD(gameState);

        EndDrawing();
    }

    // Cleanup
    UnloadSound(scream);
    UnloadSound(pew);
    UnloadSound(mew);

    CloseAudioDevice();

    CloseWindow();

    return 0;
}
