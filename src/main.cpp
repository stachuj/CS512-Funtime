#include "raylib.h"
#include "game_object.hpp"
#include "character.hpp"
#include <vector>

int main() {
    
	// list of all objects that will be updated and drawn
	// they are pointers so we can have subclasses in the vector
	std::vector<GameObject *> objects;

	// Window size
	InitWindow(800, 600, "CS512 Funtime");

	// We will draw a new frame to the screen 60 times per second
	// This will NOT limit physics to 60 ticks per second, so we have to use deltaTime!
	SetTargetFPS(60);


	// Adding a new object to the world
	// Will have to come up with a nicer way to create and remove objects from the world...
    Character* player = new Character({400, 300}, "assets");
    objects.push_back(player);

    while (!WindowShouldClose()) {

		// This is the real time in seconds since the last update
		float deltaTime = GetFrameTime();

		// For every object in objects, call their update function (passing deltaTime)
		for (auto object: objects)
			object->Update(deltaTime);
		
		BeginDrawing();
		{
			ClearBackground(RAYWHITE);

			// For every object in objects, call their draw function
			for (auto object: objects)
				object->Draw();
			
			DrawText("This is Jeremy the purple square.", 10, 10, 32, BLACK);
			
		}
		EndDrawing();
		
	}
	CloseWindow();
    return 0;
}
