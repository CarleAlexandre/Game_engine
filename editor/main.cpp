
#include <raylib.h>
#include <raymath.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

int current_level = 0;
int tool = 0;
bool saved = false;

void	startscreen() {
	BeginDrawing();
	EndDrawing();
}

int	main(void) {
	InitWindow(1820, 980, "UwUditor");
	SetTargetFPS(120);
	RenderTexture2D fbo;
	Camera2D camera = (Camera2D){
		.offset = {0, 0},
		.target = {0, 0},
		.rotation = 0,
		.zoom = 0.8,
	};

	GuiLoadStyle("include/style_cyber.rgs");

	fbo = LoadRenderTexture(1590, 960);
	while (!WindowShouldClose()) {
	}

	CloseWindow();
	return 0;
}
