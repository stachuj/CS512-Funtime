#include "game_object.hpp"
#include "test_object.hpp"
#include "tilemap.hpp"
#include "chat.hpp"
#include "character.hpp"
#include <vector>
#include <raylib.h>

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
    Sound pew = LoadSound("assets/pew.wav");
    Sound mew = LoadSound("assets/mew.wav");

    // Window size
    InitWindow(800, 600, "CS512 Funtime");
    SetTargetFPS(60);

    // Adding a new object to the world
	// Will have to come up with a nicer way to create and remove objects from the world...
    Character* player = new Character({400, 300}, GetApplicationDirectory() + std::string("assets"));
    objects.push_back(player);
    objects.push_back(new TestObject({100.0, 400.0}));

    PlaySound(scream);
    
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_UP)) PlaySound(scream);
        if (IsKeyPressed(KEY_DOWN)) PlaySound(pew);
        if (IsKeyPressed(KEY_LEFT)) PlaySound(mew);

        // This is the real time in seconds since the last update
        float deltaTime = GetFrameTime();

        // For every object in objects, call their update function (passing deltaTime)
        for (auto object: objects)
            object->Update(deltaTime);
        
        chat.Update();

        raylib::Vector2 mousePosition = GetMousePosition();
        bool mouseOnWall = isWall(mousePosition.x, mousePosition.y);
        
        BeginDrawing();
        {
            ClearBackground(RAYWHITE);

            displayTilemap();

            // For every object in objects, call their draw function
            for (auto object: objects)
                object->Draw();

            chat.Draw();
            
            //DrawText("Press 1 for Scream", 280, 150, 20, RED);
			//DrawText("Press 2 for Pew", 310, 200, 20, BLUE);
			//DrawText("Press 3 for Mew", 310, 250, 20, DARKGREEN);

			DrawText("This is Jeremy the purple square..",10, 10, 20, RAYWHITE);
			//DrawText(TextFormat("Sound device initted? %d", raylib::AudioDevice::IsReady()), 2, 2, 20, RAYWHITE);

			//raylib::DrawText(TextFormat("Mouse Position: [ X: %.0f, Y: %.0f ]", mousePosition.x, mousePosition.y), 10, 310, 32, BLACK);

			//raylib::DrawText(TextFormat("Is mouse on a wall? %s", mouseOnWall ? "true" : "false"), 10, 510, 32, BLACK);
        }
        EndDrawing();
    }

   
    
    UnloadSound(scream);
    UnloadSound(pew);
    UnloadSound(mew);
    CloseAudioDevice();
    CloseWindow();

    return 0;
}