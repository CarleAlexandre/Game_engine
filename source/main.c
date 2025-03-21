#include "engine/core.h"

#include <raylib.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

void	hot_reload_game() {

}

int main(const int ac, char *av[]) {
	haven_memory_system_init();
	haven_thread_mgr_init();
	haven_time_init();

	haven_memory_system_print();
	InitWindow(1920, 1080, "World of Haven : Chaos dungeons");
	SetWindowState(FLAG_BORDERLESS_WINDOWED_MODE | FLAG_WINDOW_RESIZABLE);

	HideCursor();
	SetTargetFPS(ac == 2 ? atoi(av[1]) : 60);

	while (!WindowShouldClose()) {
		ClearBackground(RAYWHITE);

		BeginDrawing();
			DrawFPS(10, 10);
		EndDrawing();

		haven_time_update();
		haven_stack_reset();
	}
	
	ShowCursor();

	haven_thread_mgr_close();
	haven_memory_system_shutdown();
	CloseWindow();
	return(0);
}
