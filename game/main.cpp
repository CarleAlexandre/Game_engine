#include <raylib.h>
#include <raymath.h>
#include "main.hpp"

int main(void) {
	engine_t engine;

	InitWindow(720, 480, "Game");

	engine = init_engine();

	while (!WindowShouldClose()) {
		BeginDrawing();
			BeginMode3D(engine.camera);
				ClearBackground(BLACK);
			EndMode3D();
		EndDrawing();
	}
	CloseWindow();
}
