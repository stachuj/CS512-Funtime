#include "raylib.h"
#include "basic_object.hpp"
#include "includes/tilemap.hpp"

#define MAX_OBJECTS 100

static BasicObject _objects[MAX_OBJECTS] = {0};
int objectCount = 0;

int main() {

	int tilemap[12][16] = {0} ;
	initializeTilemap((int*)tilemap, 12, 16) ;
	setTilemap((int*)tilemap, 12, 16, "testfile.txt") ;
	tilemap[0][0] = 0 ;
	getTilemap((int*)tilemap, 12, 16, "testfile.txt") ;

	InitWindow(800, 600, "CS512 Funtime");
	SetTargetFPS(60);

	Vector2 pos = {400.0,300.0};
	_objects[objectCount++] = BasicObjectCreate(pos);
	
	while (!WindowShouldClose()) {

		Vector2 mousePosition = GetMousePosition();
		bool mouseOnWall = isWall((int*)tilemap, 12, 16, mousePosition.x, mousePosition.y) ;

		for (int i = 0; i < objectCount; i++) {
			BasicObjectUpdate(&_objects[i], GetFrameTime());
		}
		
		BeginDrawing();
		{
			ClearBackground(RAYWHITE);

			displayTilemap((int*)tilemap, 12, 16) ;
			
			for (int i = 0; i < objectCount; i++) {
				BasicObjectDraw(_objects[i]);
			}

			DrawText("Hi team members...!\nDo not be slacking off.\nRemember the importance of SCRUM's principalz.", 10, 10, 32, BLACK);

			DrawText(TextFormat("Mouse Position: [ X: %.0f, Y: %.0f ]", mousePosition.x, mousePosition.y), 10, 310, 32, BLACK);

			DrawText(TextFormat("Is mouse on a wall? %s", mouseOnWall ? "true" : "false"), 10, 510, 32, BLACK);
			
		}
		EndDrawing();
		
	}
	
	CloseWindow();

    return 0;
}