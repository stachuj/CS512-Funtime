#include "raylib.h"
#include "game_object.hpp"
#include "test_object.hpp"
#include <vector>
#include "includes/tilemap.hpp"

using namespace std;

int main() {

	// list of all objects that will be updated and drawn
	// they are pointers so we can have subclasses in the vector
	vector<GameObject *> objects;

	// Window size
	int tilemap[12][16] = {0} ;
	initializeTilemap((int*)tilemap, 12, 16) ;
	setTilemap((int*)tilemap, 12, 16, "testfile.txt") ;
	tilemap[0][0] = 0 ;
	getTilemap((int*)tilemap, 12, 16, "testfile.txt") ;

	InitWindow(800, 600, "CS512 Funtime");

	// We will draw a new frame to the screen 60 times per second
	// This will NOT limit physics to 60 ticks per second, so we have to use deltaTime!
	SetTargetFPS(60);

	// Adding a new object to the world
	// Will have to come up with a nicer way to create and remove objects from the world...
	objects.push_back(new TestObject({100.0, 400.0}));
	
	while (!WindowShouldClose()) {

		// This is the real time in seconds since the last update
		float deltaTime = GetFrameTime();

		// For every object in objects, call their update function (passing deltaTime)
		for (auto object: objects)
			object->Update(deltaTime);
		Vector2 mousePosition = GetMousePosition();
		bool mouseOnWall = isWall((int*)tilemap, 12, 16, mousePosition.x, mousePosition.y) ;
/*
		for (int i = 0; i < objectCount; i++) {
			BasicObjectUpdate(&_objects[i], GetFrameTime());
		}
			*/
		
		BeginDrawing();
		{
			ClearBackground(RAYWHITE);

			displayTilemap((int*)tilemap, 12, 16) ;

			// For every object in objects, call their draw function
			for (auto object: objects)
				object->Draw();
			
			DrawText("This is Jeremy the purple square.", 10, 10, 32, BLACK);
			/*
			for (int i = 0; i < objectCount; i++) {
				BasicObjectDraw(_objects[i]);
			}
				*/

			DrawText("Hi team members...!\nDo not be slacking off.\nRemember the importance of SCRUM's principalz.", 10, 10, 32, BLACK);

			DrawText(TextFormat("Mouse Position: [ X: %.0f, Y: %.0f ]", mousePosition.x, mousePosition.y), 10, 310, 32, BLACK);

			DrawText(TextFormat("Is mouse on a wall? %s", mouseOnWall ? "true" : "false"), 10, 510, 32, BLACK);
			
		}
		EndDrawing();
		
	}
	CloseWindow();

    return 0;
}