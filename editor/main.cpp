
#include <engine.hpp>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

int	tool = 0;
bool	saved = false;
int	editor_mode = 0;
bool	mode_enable = false;
int	error = 0;
Rectangle error_bound = (Rectangle){};

void	startscreen() {
	BeginDrawing();
	EndDrawing();
}

// ui

void	top_bar() {
	GuiStatusBar({0, 0, (float)GetScreenWidth(), 30}, "");
	if (GuiDropdownBox({0, 0, 120, 30}, "map;item;obj;ui", &editor_mode, mode_enable))
		mode_enable = !mode_enable;
}

void	left_bar() {
	GuiDummyRec({0, 30, 200, (float)GetScreenHeight() - 30}, "Debug");
}

void	right_bar() {
	GuiDummyRec({(float)GetScreenWidth() - 200, 30, 200, (float)GetScreenHeight() - 30}, "setting");
}

//editor mode

void	map_editor() {
	static Camera2D cam = (Camera2D){
		.offset = {0, 0},
		.target = {0, 0},
		.rotation = 0,
		.zoom = 0.8,
	};

	BeginMode2D(cam);
	EndMode2D();
}

void	item_editor() {

}

void	obj_editor() {

}

void	ui_editor() {

}

//

int	main(void) {

	InitWindow(1920, 1080, "UwUditor");

	RenderTexture2D fbo;
	GuiLoadStyle("include/style_terminal.rgs");
	fbo = LoadRenderTexture(GetScreenWidth() - 400, GetScreenHeight() - 30);

	SetTargetFPS(30);
	EnableEventWaiting();
	while (!WindowShouldClose()) {

		BeginTextureMode(fbo);
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
		EndTextureMode();


		BeginDrawing();
			ClearBackground(BLACK);

			DrawTextureRec(fbo.texture, (Rectangle){0, 0, (float)fbo.texture.width, (float)-fbo.texture.height}, {200, 30}, WHITE);

			right_bar();
			left_bar();

			switch (error) {
				case (error_file_not_found): {
					int result = GuiMessageBox(error_bound, "#152#ERROR!", "this file doesn't exist!!", "OK");
					if (result == 1) {
						error = error_dummy;
					}
					break;
				}
				case (error_file_empty): {
					int result = GuiMessageBox(error_bound, "#152#ERROR!", "Name Empty!!", "OK");
					if (result == 1) {
						error = error_dummy;
					}
					break;
				}
				case (error_file_corrupted): {
					int result = GuiMessageBox(error_bound, "#152#ERROR!", "this file is corrupted!!", "OK");
					if (result == 1) {
						error = error_dummy;
					}
					break;
				}
				case (error_file_exist): {
					int result = GuiMessageBox(error_bound, "#152#ERROR!", "this filename already exist!!", "OK");
					if (result == 1) {
						error = error_dummy;
					}
					break;
				}
				default:break;
			}

			top_bar();

		EndDrawing();
	}

	CloseWindow();
	return 0;
}
