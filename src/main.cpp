#include "raylib.h"
#include "basic_object.hpp"

#define MAX_OBJECTS 100

static BasicObject _objects[MAX_OBJECTS] = {0};
int objectCount = 0;

int main() {

	InitWindow(800, 600, "CS512 Funtime");
	SetTargetFPS(60);

	Vector2 pos = {400.0,300.0};
	_objects[objectCount++] = BasicObjectCreate(pos);
	
	while (!WindowShouldClose()) {

		for (int i = 0; i < objectCount; i++) {
			BasicObjectUpdate(&_objects[i], GetFrameTime());
		}
		
		BeginDrawing();
		{
			ClearBackground(RAYWHITE);
			
			for (int i = 0; i < objectCount; i++) {
				BasicObjectDraw(_objects[i]);
			}

			DrawText("Hi team members...!\nDo not be slacking off.\nRemember the importance of SCRUM's principalz.", 10, 10, 32, BLACK);
			
		}
		EndDrawing();
		
	}
	
	CloseWindow();

    return 0;
}