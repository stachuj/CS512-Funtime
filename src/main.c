#include "raylib.h"

int main() {

	InitWindow(800, 600, "CS512 Funtime");
	SetTargetFPS(60);
	
	while (!WindowShouldClose()) {
		
		BeginDrawing();
		{
			ClearBackground(RAYWHITE);
			
			DrawText("Hi team members...!\nDo not be slacking off.\nRemember the importance of SCRUM's principalz.", 10, 10, 32, BLACK);
			
		}
		EndDrawing();
		
	}
	
	CloseWindow();

    return 0;
}