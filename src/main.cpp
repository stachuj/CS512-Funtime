#include "game_object.hpp"
#include "test_object.hpp"
#include "tilemap.hpp"
#include "chat.hpp"
#include "character.hpp"
#include <vector>
#include <raylib.h>

#include "GameState.h"     // ADDED (score/timer state)
#include "HUD.h"           // ADDED (draw score/timer)
#include "Collectible.h"   // ADDED (collectibles)
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

    // list of all objects that will be updated and drawn
    // they are pointers so we can have subclasses in the vector
    std::vector<GameObject *> objects;

    initializeTilemap();
    //setTilemap("testfile.txt");
    //getTilemap("testfile.txt");

    InitAudioDevice();

    Sound scream = LoadSound("assets/scream.wav");
    Sound pew    = LoadSound("assets/pew.wav");
    Sound mew    = LoadSound("assets/mew.wav");

    // ADDED: pickup SFX for collectibles (reuse existing)
    Sound pickupSfx = pew;

    // Window size
    InitWindow(800, 600, "CS512 Funtime");
    SetTargetFPS(60);

    // Adding a new object to the world
    // Will have to come up with a nicer way to create and remove objects from the world...
    Character* player = new Character({400, 300}, GetApplicationDirectory() + std::string("assets"));
    objects.push_back(player);
    objects.push_back(new TestObject({100.0, 400.0}));

    // ADDED: game state + collectibles
    GameState gs;                               // holds score & timer
    std::vector<Collectible> collectibles;      // pickups to score
    Collectibles::SpawnRandom(collectibles, 10, {0, 0, (float)GetScreenWidth(), (float)GetScreenHeight()});

    // ADDED: chat toggle (hidden by default)
    bool chatVisible = false;

    PlaySound(scream);
    
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_UP))   PlaySound(scream);
        if (IsKeyPressed(KEY_DOWN)) PlaySound(pew);
        if (IsKeyPressed(KEY_LEFT)) PlaySound(mew);

        // Toggle chat overlay with C
        if (IsKeyPressed(KEY_C)) chatVisible = !chatVisible;

        // This is the real time in seconds since the last update
        float deltaTime = GetFrameTime();

        // ADDED: countdown timer
        if (!gs.timeUp) {
            gs.timeRemaining -= deltaTime;
            if (gs.timeRemaining <= 0.0f) {
                gs.timeRemaining = 0.0f;
                gs.timeUp = true;
            }
        }

        // Update game objects
        for (auto object: objects)
            object->Update(deltaTime);
        
        // Chat update (kept)
        chat.Update();

        Vector2 mousePosition = GetMousePosition();
        bool mouseOnWall = isWall(mousePosition.x, mousePosition.y);

        // ADDED: player bounds for pickup collision
        Rectangle playerBounds;
        Vector2 p  = player->GetPosition();
        Vector2 sz = player->GetSize();
        playerBounds = { p.x - sz.x * 0.5f, p.y - sz.y * 0.5f, sz.x, sz.y };

        if (!gs.timeUp) {
            int newlyPicked = Collectibles::Update(collectibles, &playerBounds, pickupSfx);
            gs.score += newlyPicked * gs.pointsPerCollectible;
        }

        // Optional restart after time up
        if (gs.timeUp && IsKeyPressed(KEY_R)) {
            gs.score = 0;
            gs.timeRemaining = gs.timeLimit;
            gs.timeUp = false;
            for (auto& c : collectibles) c.active = true; // reactivate all
        }
        
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            // ---- WORLD (no camera mode here) ----
            displayTilemap();
            for (auto object: objects)
                object->Draw();

            // ---- UI (screen space) ----
            // Draw collectibles ABOVE world (so always visible), then HUD, then optional chat on top
            Collectibles::Draw(collectibles);

            // Draw HUD before chat so chat panel never covers score/time
            DrawHUD(gs);

            // Only draw the chat overlay if toggled on (press C)
            if (chatVisible) {
                chat.Draw();
            }

            DrawText("This is Jeremy the purple square..", 10, 10, 20, BLACK); // BLACK for readability
            DrawText("Press C to toggle chat", 10, 36, 16, DARKGRAY);
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
