#include "raylib.h"
#include "includes/tilemap.h"
#include <stdio.h>


int main() {

	int tilemap[16][12] = {0} ;
	initializeTilemap((int*)tilemap, 16, 12) ;

	InitWindow(800, 600, "CS512 Funtime");
	SetTargetFPS(60);
	
	while (!WindowShouldClose()) {
		
		BeginDrawing();
		{
			ClearBackground(RAYWHITE);

			displayTilemap((int*)tilemap, 16, 12) ;
			
			// DrawText("Hi team members...!\nDo not be slacking off.\nRemember the importance of SCRUM's principalz.", 10, 10, 32, BLACK);
			
		}
		EndDrawing();
		
	}
	
	CloseWindow();

    return 0;
}