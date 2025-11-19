#include "game_object.hpp"
#include "test_object.hpp"
#include "tilemap.hpp"
#include "chat.hpp"
#include "character.hpp"
#include "test_object_A_Star.hpp"
#include "level_manager.hpp"
#include "level_select.hpp"

#include "raylib-cpp.hpp"
#include <vector>

#include "game_state.hpp"     
#include "hud.hpp"           
#include "collectible.hpp"  
#include <string>

#include "menu.hpp"

#define WINDOW_WIDTH 1024;
#define WINDOW_HEIGHT 768;

int lastLevel = 2;

Texture2D enemyTex = { 0 }; 

using namespace std;

GameState gameState;                   // holds score & timer
Character* playerPtr;
vector<GameObject *> enemies;
vector<Collectible> collectibles;      // pickups to score

void GoToLevel(int num);
void InitLevel();

int main() {

    // Load levels
    LoadFromFile("../../src/levels/testLevels.txt") ;
    LoadLevel(0);

    InitAudioDevice();
    Sound scream = LoadSound("../../assets/scream.wav");
    Sound pickup = LoadSound("../../assets/pickup.mp3");
    Sound mew = LoadSound("../../assets/mew.wav");


    InitWindow(1024, 768, "CS512 Funtime");
    SetTargetFPS(60);

    enemyTex = LoadTexture("../../assets/zombie.png"); //SPRITE

    playerPtr = Character::GetPlayer();

    GameStates currentState = GameStates::Menu;
    Menu mainMenu(MenuType::Main);
    Menu pauseMenu(MenuType::Pause);
    Menu settingsMenu(MenuType::Settings);
    MenuType lastMenuType = MenuType::Main;  // remembers where rules came from
    LevelSelect levelSelectMenu ;

    int editorSelection = 1;
    int levelIndex = 0;
    
    bool testing = false;

    string selectionNames[4] = {
        "Wall",
        "Coin",
        "Enemy",
        "Player Start"    
    };

    Rectangle pauseBtn = {1024 - 100, 768 - 50, 80, 30};
    Rectangle returnBtn = {1024 - 180, 768 - 60, 160, 40};

    while (!WindowShouldClose() && currentState!= GameStates::Exit) {

        switch (currentState) {
            case GameStates::Menu: {
                MenuResult result = mainMenu.Update();

                if (result == MenuResult::StartGame) {
                    // Change this to be GameStates::LevelSelect
                    // currentState = GameStates::Game;
                    // GoToLevel(levelIndex);
                    currentState = GameStates::LevelSelect;
                }
                else if (result == MenuResult::StartEditor)
                    currentState = GameStates::Editor;
                else if (result == MenuResult::Rules) {
                    currentState = GameStates::Rules;
                    lastMenuType = MenuType::Main;
                }
                else if(result == MenuResult::Settings)
                    currentState = GameStates::Settings;
                else if (result == MenuResult::Exit)
                    currentState = GameStates::Exit;
            } break;

            case GameStates::LevelSelect: {
                int result = levelSelectMenu.Update() ;
                if (result == 1) {
                    currentState = GameStates::Game ;
                    levelIndex = levelSelectMenu.GetLevel() ;
                    GoToLevel(levelIndex) ;
                }
                else if (result == 2) {
                    currentState = GameStates::Menu ;
                }
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
                    int newlyPicked = Collectibles::Update(collectibles, &playerBox, pickup);
                    gameState.score += newlyPicked * gameState.pointsPerCollectible;
                }

                bool levelFinished = true;
                for (Collectible c: collectibles) {
                    if (c.active)
                        levelFinished = false;
                }

                if (levelFinished) {
                    if(testing == true) {
                        testing = false ;
                        currentState = GameStates::Editor;
                    }
                    else {
                        currentState = GameStates::LevelBeat;
                    }
                }

                if (playerPtr->dead) {
                    PlaySound(scream);
                    playerPtr->dead = false;
                    gameState.score = 0;
                    if(testing == true) {
                        testing = false ;
                        currentState = GameStates::Editor;
                    }
                    else {
                        GoToLevel(levelIndex);
                    }
                }

                // If clicked -> go to pause menu
                if (CheckCollisionPointRec(GetMousePosition(), pauseBtn) &&
                    IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
                    testing == false) {
                    currentState = GameStates::Pause;
                }

                if(IsKeyPressed(KEY_T)) {
                    if(testing == true) {
                        testing = false ;
                        currentState = GameStates::Editor;
                    }
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

                if(IsKeyPressed(KEY_T)) {
                    testing = true;
                    currentState = GameStates::Game;
                    InitLevel() ;
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

            case GameStates::Settings: {
                if (settingsMenu.Update() == MenuResult::Resume)
                    currentState = GameStates::Menu;
            } break;

            case GameStates::LevelBeat: {
                if (levelIndex+1 != lastLevel && IsKeyPressed(KEY_ENTER)) {
                    currentState = GameStates::Game;
                    GoToLevel(++levelIndex);
                }
            } break;

            default: break;
        }

        BeginDrawing();


        switch (currentState) {

            case GameStates::LevelBeat: {
                ClearBackground(BLACK);
                DrawText("YOU BEAT THE LEVEL!", 240, 200, 48, WHITE);
                if (levelIndex+1 != lastLevel)
                    DrawText("Press enter to continue....", 300, 400, 32, WHITE);
                else
                    DrawText("And you beat the whole game too, good job.", 180, 400, 32, WHITE);
            } break;


            case GameStates::Menu: {
                mainMenu.Draw();
            } break;

            case GameStates::LevelSelect: {
                levelSelectMenu.Draw();
            } break;

            case GameStates::Game: {
                ClearBackground(RAYWHITE);

                Texture2D floorTexture = LoadTexture("../../assets/floor.png");

                for (int i = 0; i < 12; i++) {
                    for (int j = 0; j < 16; j++) {
                        DrawTexture(floorTexture, j * 64, i * 64, WHITE);
                    }
                }

                displayTilemap();

                for (auto enemy: enemies)
                    enemy->Draw();

                Collectibles::Draw(collectibles);

                playerPtr->Draw();

                DrawHUD(gameState);

                // Pause button bottom-right
                // Only in actual game and not in editor testing

                if(testing == false) {
                    DrawRectangleRec(pauseBtn, DARKGRAY);
                    DrawText("Pause", pauseBtn.x + 10, pauseBtn.y + 5, 20, WHITE);
                }

            } break;

            case GameStates::Editor: {
                ClearBackground(RAYWHITE);

                displayTilemapEditor();

                DrawText("Left click - place; Right click - delete; Q - change selection; Left/right - Change level; Ctrl+S - Save", 20, 20, 18, GREEN);
                DrawText("T - Test level; M - Return to Main Menu", 20, 40, 18, GREEN);
                DrawText(("Selection " + to_string(editorSelection) + ": " + selectionNames[editorSelection - 1]).c_str(), 20, 60, 20, GREEN);
                DrawText(("Current Level: " + to_string(levelIndex + 1)).c_str(), 20, 80, 20, GREEN);

            } break;

            case GameStates::Pause: {
                pauseMenu.Draw();
            } break;

            case GameStates::Rules: {
                ClearBackground(DARKBLUE);
                DrawText("GAME RULES", 340, 100, 40, RAYWHITE);
                DrawText("Rule 1. Use W, A, S, D to move your creature.", 80, 200, 25, RAYWHITE);
                DrawText("Rule 2. Collect all yellow circles to advance to the next stage.", 70, 280, 25, RAYWHITE);
                DrawText("Rule 3. Avoid the hungry zombie fellows. They will awake when you draw near.", 60, 360, 25, RAYWHITE);

                DrawRectangleRec(returnBtn, DARKGRAY);
                DrawText("Return", returnBtn.x + 30, returnBtn.y + 10, 25, WHITE);

            } break;

            case GameStates::Settings: {
                settingsMenu.Draw();
            } break;

            default: break;
        }



        EndDrawing();
    }

    // Cleanup
    UnloadSound(scream);
    UnloadSound(pickup);
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
                   TestObjectAStar* e = new TestObjectAStar({col * 64.0f + 32, row * 64.0f + 32});
                    // assign shared spritesheet; false = don't UnloadTexture in destructor
                    e->setSpriteSheet(enemyTex, false);
                    enemies.push_back(e);;
                }

                if (currTile == 4) {
                    playerPtr->SetPosition({col * 64.0f + 32, row * 64.0f + 32});
                }

                // setTile(row, col, 0);

            }





        }
    }
}