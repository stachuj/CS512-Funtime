#include "game_object.hpp"
#include "test_object.hpp"
#include "tilemap.hpp"
#include "chat.hpp"
#include "character.hpp"
#include "test_object_A_Star.hpp"
#include "menu.hpp"
#include <vector>
#include <raylib.h>
#include "GameState.h"   
#include "HUD.h"
#include "Collectible.h"
#include <string>

using namespace std;

Chat chat;

std::string GetAssetsPath() {
    static std::string assetsPath;
    if (assetsPath.empty()) {
        assetsPath = GetApplicationDirectory();
        assetsPath += "assets/";
    }
    return assetsPath;
}

int main() {
    // Window initialization
    InitWindow(800, 600, "CS512 Funtime");
    SetTargetFPS(60);
    InitAudioDevice();

    // variables for every sstate
    enum class AppState {
        MAIN_MENU,
        PLAYING,
        PAUSED,
        RULES,
        GAME_OVER
    };
    
    AppState currentAppState = AppState::MAIN_MENU;
    Menu mainMenu(MenuType::Main);
    Menu pauseMenu(MenuType::Pause);
    
    // Game objects
    std::vector<GameObject *> objects;
    initializeTilemap();
    initializeAStarTestTilemap();

    // AStar
    std::stack<Pair> Path;
    float testEnemyUpdate = 3.0;
    int targetRowTile = 0;
    int targetColTile = 0;

    // Load sounds
    Sound scream = LoadSound("assets/scream.wav");
    Sound pew    = LoadSound("assets/pew.wav");
    Sound mew    = LoadSound("assets/mew.wav");
    Sound pickupSfx = pew;

    // Create game objects
    Character* player = new Character({500, 300}, GetApplicationDirectory() + std::string("assets"));
    objects.push_back(player);
    objects.push_back(new TestObject({100.0, 400.0}));
    TestObjectAStar* testEnemy = new TestObjectAStar({80.0, 80.0});
    objects.push_back(testEnemy);

    // Game state
    GameState gameStats;  
    std::vector<Collectible> collectibles;
    Collectibles::SpawnRandom(collectibles, 10, {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()});

    bool chatVisible = false;
    PlaySound(scream);

    // Main game loop
    while (!WindowShouldClose()) {
        float deltaTime = GetFrameTime();

        // State machine
        switch (currentAppState) {
            case AppState::MAIN_MENU: {
                MenuResult result = mainMenu.Update();
                if (result == MenuResult::StartGame) {
                    currentAppState = AppState::PLAYING;
                    gameStats.score = 0;
                    gameStats.timeRemaining = gameStats.timeLimit;
                    gameStats.timeUp = false;
                    for (auto& c : collectibles) c.active = true;
                } else if (result == MenuResult::Rules) {
                    currentAppState = AppState::RULES;
                } else if (result == MenuResult::Exit) {
                    break; // Exit game
                }
                break;
            }

            case AppState::PLAYING: {
                // PAUSE BUTTON
                if (IsKeyPressed(KEY_P)) {
                    currentAppState = AppState::PAUSED;
                    break;
                }

                // Chat, probably should delete
                if (IsKeyPressed(KEY_C)) chatVisible = !chatVisible;

                // Timer
                if (!gameStats.timeUp) {
                    gameStats.timeRemaining -= deltaTime;
                    if (gameStats.timeRemaining <= 0.0f) {
                        gameStats.timeRemaining = 0.0f;
                        gameStats.timeUp = true;
                        currentAppState = AppState::GAME_OVER;
                    }
                }

                // Astar code for enemy chasing player's position
                testEnemyUpdate -= deltaTime;
                Vector2 wasdCharacterPos = player->GetPosition();
                if (!isWall(wasdCharacterPos.x, wasdCharacterPos.y)) {
                    targetColTile = getTilePos(wasdCharacterPos.x);
                    targetRowTile = getTilePos(wasdCharacterPos.y);
                }
                if (testEnemyUpdate <= 0.0) {
                    testEnemyUpdate = 3.0;
                    int testEnemyColTile = getTilePos(testEnemy->position.x);
                    int testEnemyRowTile = getTilePos(testEnemy->position.y);
                    Path = AStarSearch(testEnemyRowTile, testEnemyColTile, targetRowTile, targetColTile);
                    testEnemy->setPath(Path);
                }

                // Update game objects
                for (auto object : objects)
                    object->Update(deltaTime);
                
                // Chat update
                chat.Update();

                // Player bounds for pickup collision
                Rectangle playerBounds;
                Vector2 p = player->GetPosition();
                Vector2 sz = player->GetSize();
                playerBounds = { p.x - sz.x * 0.5f, p.y - sz.y * 0.5f, sz.x, sz.y };

                // Update collectibles
                if (!gameStats.timeUp) {
                    int newlyPicked = Collectibles::Update(collectibles, &playerBounds, pickupSfx);
                    gameStats.score += newlyPicked * gameStats.pointsPerCollectible;
                }

                break;
            }

            case AppState::PAUSED: {
                MenuResult result = pauseMenu.Update();
                if (result == MenuResult::Resume) {
                    currentAppState = AppState::PLAYING;
                } else if (result == MenuResult::Rules) {
                    currentAppState = AppState::RULES;
                } else if (result == MenuResult::Exit) {
                    currentAppState = AppState::MAIN_MENU;
                }
                break;
            }

            case AppState::RULES: {
                if (IsKeyPressed(KEY_P) || IsKeyPressed(KEY_ENTER)) {
                    currentAppState = AppState::MAIN_MENU;
                }
                break;
            }

            case AppState::GAME_OVER: {
                if (IsKeyPressed(KEY_ENTER)) {
                    currentAppState = AppState::MAIN_MENU;
                } else if (IsKeyPressed(KEY_R)) {
                    currentAppState = AppState::PLAYING;
                    gameStats.score = 0;
                    gameStats.timeRemaining = gameStats.timeLimit;
                    gameStats.timeUp = false;
                    for (auto& c : collectibles) c.active = true;
                }
                break;
            }
        }

        // Rendering
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            switch (currentAppState) {
                case AppState::MAIN_MENU:
                    mainMenu.Draw();
                    break;

                case AppState::PLAYING:

                    displayPath(Path);
                    displayTilemap();
                    for (auto object : objects)
                        object->Draw();
                    Collectibles::Draw(collectibles);
                    DrawHUD(gameStats);  
                    if (chatVisible) chat.Draw();
                    DrawText("This is Jeremy the purple square..", 10, 10, 20, BLACK);
                    DrawText("Press C to toggle chat", 10, 36, 16, DARKGRAY);
                    break;

                case AppState::PAUSED:
                    displayTilemap();
                    for (auto object : objects)
                        object->Draw();
                    Collectibles::Draw(collectibles);
                    DrawHUD(gameStats); 
                    // Transparent wile paused
                    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), {0, 0, 0, 128});
                    pauseMenu.Draw();
                    break;

                case AppState::RULES:
                    ClearBackground(BLACK);
                    DrawText("GAME RULES", 250, 100, 40, RAYWHITE);
                    DrawText("1. Collect all items before time runs out", 100, 200, 24, RAYWHITE);
                    DrawText("2. Avoid the enemy chasing you", 100, 240, 24, RAYWHITE);
                    DrawText("3. Use WASD to move your character", 100, 280, 24, RAYWHITE);
                    DrawText("4. Press P to pause the game", 100, 320, 24, RAYWHITE);
                    DrawText("Press P or ENTER to go back", 200, 450, 24, GRAY);
                    break;

                case AppState::GAME_OVER:
                    ClearBackground(BLACK);
                    DrawText("GAME OVER", 280, 150, 40, RAYWHITE);
                    DrawText(TextFormat("Final Score: %d", gameStats.score), 300, 250, 30, RAYWHITE);
                    DrawText("Press ENTER for Main Menu", 250, 350, 24, GRAY);
                    DrawText("Press R to Restart", 280, 400, 24, GRAY);
                    break;
            }
        }
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