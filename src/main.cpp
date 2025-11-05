#include "game_object.hpp"
#include "test_object.hpp"
#include "tilemap.hpp"
#include "chat.hpp"
#include "character.hpp"
#include "test_object_A_Star.hpp"
#include "level_manager.hpp"

#include "raylib-cpp.hpp"
#include <vector>

#include "game_state.hpp"     
#include "hud.hpp"           
#include "collectible.hpp"  
#include <string>

#include "menu.hpp"

#define WINDOW_WIDTH 1024;
#define WINDOW_HEIGHT 768;

using namespace std;

GameState gameState;                   // holds score & timer
Character* playerPtr;
vector<GameObject *> enemies;
std::vector<Collectible> collectibles;      // pickups to score

void GoToLevel(int num);
void InitLevel();

int main() {

    // Load levels
    LoadFromFile("../../src/levels/testLevels.txt") ;
    LoadLevel(0);

    InitAudioDevice();
    Sound scream = LoadSound("../../assets/scream.wav");
    Sound pew = LoadSound("../../assets/pew.wav");
    Sound mew = LoadSound("../../assets/mew.wav");
    Sound pickupSfx = pew;

    InitWindow(1024, 768, "CS512 Funtime");
    SetTargetFPS(60);

    playerPtr = Character::GetPlayer();

    GameStates currentState = GameStates::Menu;
    Menu mainMenu(MenuType::Main);
    Menu pauseMenu(MenuType::Pause);
    MenuType lastMenuType = MenuType::Main;  // remembers where rules came from

    int editorSelection = 1;
    int levelIndex = 0;

    string selectionNames[4] = {
        "Wall",
        "Coin",
        "Enemy",
        "Player Start"    
    };

    Rectangle pauseBtn = {1024 - 100, 768 - 50, 80, 30};
    Rectangle returnBtn = {1024 - 180, 768 - 60, 160, 40};

    //Character::GetPlayer()->SetPosition({400.0, 400.0});

    while (!WindowShouldClose() && currentState!= GameStates::Exit) {

        switch (currentState) {
            case GameStates::Menu: {
                MenuResult result = mainMenu.Update();

                if (result == MenuResult::StartGame) {
                    currentState = GameStates::Game;
                    GoToLevel(levelIndex);
                }
                else if (result == MenuResult::StartEditor)
                    currentState = GameStates::Editor;
                else if (result == MenuResult::Rules) {
                    currentState = GameStates::Rules;
                    lastMenuType = MenuType::Main;
                } else if (result == MenuResult::Exit)
                    currentState = GameStates::Exit;
            } break;

            case GameStates::Game: {

                // This is the real time in seconds since the last update
                float deltaTime = GetFrameTime();

                gameState.UpdateTimer(deltaTime);

                if (gameState.timeUp)
                    playerPtr->dead = true;

                playerPtr->Update(deltaTime);

                for (auto enemy: enemies)
                    enemy->Update(deltaTime);

                Rectangle playerBox = playerPtr->GetCollisionBox();

                //collecting coin behavior
                if (!gameState.timeUp) {
                    int newlyPicked = Collectibles::Update(collectibles, &playerBox, pickupSfx);
                    gameState.score += newlyPicked * gameState.pointsPerCollectible;
                }

                bool levelFinished = true;
                for (Collectible c: collectibles) {
                    if (c.active)
                        levelFinished = false;
                }

                if (levelFinished) {
                    GoToLevel(++levelIndex);
                }

                if (playerPtr->dead) {
                    playerPtr->dead = false;
                    gameState.score = 0;
                    GoToLevel(levelIndex);
                }

                // If clicked -> go to pause menu
                if (CheckCollisionPointRec(GetMousePosition(), pauseBtn) &&
                    IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    currentState = GameStates::Pause;
                }
            } break;

            case GameStates::Editor: {

                if(IsKeyPressed(KEY_Q)) {
                    editorSelection +=1;
                    if(editorSelection == 5)
                        editorSelection = 1;
                }

                Vector2 mousePos = GetMousePosition();

                if(IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                    setTile(getTilePos(mousePos.y), getTilePos(mousePos.x), editorSelection);
                }

                if(IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
                    setTile(getTilePos(mousePos.y), getTilePos(mousePos.x), 0);
                }

                if(IsKeyPressed(KEY_ESCAPE)) {
                    currentState = GameStates::Exit;
                }

                //TODO!

                if(IsKeyDown(KEY_LEFT_CONTROL) && IsKeyPressed(KEY_S)) {
                    // Save the tilemap to the list of levels
                    // And save the list of levels to file
                    SaveLevel(levelIndex) ;
                    SaveToFile("../../src/levels/testLevels.txt") ;
                }

                if(IsKeyPressed(KEY_LEFT)) {
                    //Go to the previous level (current level-1)
                    //This will discard changes unless saved
                    levelIndex -= 1 ;
                    if(levelIndex == -1)
                        levelIndex = 29 ;
                    LoadLevel(levelIndex) ;
                    
                }

                if(IsKeyPressed(KEY_RIGHT)) {
                    //Go to the next level (current level+1)
                    //This will discard changes unless saved
                    levelIndex += 1 ;
                    if(levelIndex == 30)
                        levelIndex = 0 ;
                    LoadLevel(levelIndex) ;
                }

                if(IsKeyPressed(KEY_M)) {
                    currentState = GameStates::Menu;
                }


            } break;

            case GameStates::Pause: {
                MenuResult result = pauseMenu.Update();

                if (result == MenuResult::Resume)
                    currentState = GameStates::Game;
                else if (result == MenuResult::StartEditor)
                    currentState = GameStates::Editor;
                else if (result == MenuResult::Rules) {
                    currentState = GameStates::Rules;
                    lastMenuType = MenuType::Pause;
                } else if (result == MenuResult::Exit)
                    currentState = GameStates::Exit;
            } break;

            case GameStates::Rules: {

                

                if (CheckCollisionPointRec(GetMousePosition(), returnBtn) &&
                    IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                    // Return to previous menu
                    currentState = (lastMenuType == MenuType::Pause)
                        ? GameStates::Pause
                        : GameStates::Menu;
                }
            } break;

            default: break;
        }

        BeginDrawing();


        switch (currentState) {
            case GameStates::Menu: {
                mainMenu.Draw();
            } break;

            case GameStates::Game: {
                ClearBackground(RAYWHITE);

                displayTilemap();

                for (auto enemy: enemies)
                    enemy->Draw();

                Collectibles::Draw(collectibles);

                playerPtr->Draw();

                DrawHUD(gameState);

                // Pause button bottom-right
                
                DrawRectangleRec(pauseBtn, DARKGRAY);
                DrawText("Pause", pauseBtn.x + 10, pauseBtn.y + 5, 20, WHITE);

            } break;

            case GameStates::Editor: {
                ClearBackground(RAYWHITE);

                displayTilemap();

                DrawText("Left click - place; Right click - delete; Q - change selection; Left/right - Change level; Ctrl+S - Save", 20, 20, 18, GREEN);
                DrawText(("Selection " + to_string(editorSelection) + ": " + selectionNames[editorSelection - 1]).c_str(), 20, 40, 20, GREEN);
                DrawText(("Current Level: " + to_string(levelIndex + 1)).c_str(), 20, 60, 20, GREEN);

            } break;

            case GameStates::Pause: {
                pauseMenu.Draw();
            } break;

            case GameStates::Rules: {
                ClearBackground(DARKBLUE);
                DrawText("GAME RULES", 300, 120, 40, RAYWHITE);
                DrawText("1. Rule 1", 240, 200, 25, RAYWHITE);
                DrawText("2. Rule 2", 260, 240, 25, RAYWHITE);
                DrawText("3. Rule 3", 280, 280, 25, RAYWHITE);

                DrawRectangleRec(returnBtn, DARKGRAY);
                DrawText("Return", returnBtn.x + 30, returnBtn.y + 10, 25, WHITE);

            } break;

            default: break;
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

void GoToLevel(int num) {
    LoadLevel(num);
    InitLevel();
}

void InitLevel() {

    int currTile = -1;

    gameState.ResetTimer();
    enemies.clear();
    collectibles.clear();

    for(int row = 0 ; row < 12 ; row++) {
        for(int col = 0 ; col < 16 ; col++) {

            currTile = getTile(row, col);

            if (currTile > 1) {

                if (currTile == 2) {
                    Collectible c;
                    c.pos = {col * 64.0f + 32, row * 64.0f + 32};
                    collectibles.push_back(c);
                }

                if (currTile == 3) {
                    enemies.push_back(new TestObjectAStar({col * 64.0f + 32, row * 64.0f + 32}));
                }

                if (currTile == 4) {
                    playerPtr->SetPosition({col * 64.0f + 32, row * 64.0f + 32});
                }

                setTile(row, col, 0);

            }





        }
    }
}