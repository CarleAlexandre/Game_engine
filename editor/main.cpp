
#include <engine.hpp>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

int	tool = 0;
bool	saved = false;
int	editor_mode = 0;

void	startscreen() {
	BeginDrawing();
	EndDrawing();
}

// ui

void	top_bar() {
	bool mode_enable = false;
	
	GuiStatusBar({0, 0, (float)GetScreenWidth(), 30}, "");
	if (GuiDropdownBox({0, 0, 120, 30}, "map;item;obj;ui", &editor_mode, mode_enable)) {
		mode_enable != mode_enable;
	}
}

void	left_bar() {
	GuiDummyRec({0, 30, 100, (float)GetScreenHeight()}, "Debug");
}

void	right_bar() {
	GuiDummyRec({(float)GetScreenWidth() - 100, 30, 100, (float)GetScreenHeight()}, "setting");
}

//editor mode

void	map_editor() {
}

void	item_editor() {

}

void	obj_editor() {

}

void	ui_editor() {

}

//

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

	GuiLoadStyle("include/style_lavanda.rgs");

	fbo = LoadRenderTexture(1590, 960);
	while (!WindowShouldClose()) {

		BeginDrawing();

			top_bar();
			left_bar();
			right_bar();

			BeginScissorMode(100, 30, GetScreenWidth() - 200, GetScreenHeight() - 30);
			switch (editor_mode) {
				case (0): {
					map_editor();
					break;
				}
				case (2): {
					item_editor();
					break;
				}
				case (3): {
					obj_editor();
					break;
				}
				case (1): {
					ui_editor();
					break;
				}
				default:break;
			}
			EndScissorMode();

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
