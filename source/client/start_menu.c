#include "game.h"
#include "../engine/render/gui.h"

#define BUTTON_NUMBER 4

static haven_gui_button *button[BUTTON_NUMBER];

void	game_menu_start_init(void) {
	button[0] = haven_gui_button_create("Offline", (Rectangle){20, 100, 200, 50}, TEXTURE_GUI_BUTTON);
	button[1] = haven_gui_button_create("Online", (Rectangle){20, 200, 200, 50}, TEXTURE_GUI_BUTTON);
	button[2] = haven_gui_button_create("Setting", (Rectangle){20, 300, 200, 50}, TEXTURE_GUI_BUTTON);
	button[3] = haven_gui_button_create("Leave", (Rectangle){20, 400, 200, 50}, TEXTURE_GUI_BUTTON);
}

int	game_menu_start_update() {
	Vector2 mouse_pos = GetMousePosition();
	for (int i = 0; i <  BUTTON_NUMBER; i++) {
		if (haven_gui_button_logic(button[i], mouse_pos, haven_sound_play, SOUND_MENU_CLICK1)) {
			haven_sound_play(SOUND_MENU_CLICK2);
			return (i + 1);
		}
	}
	return (0);
}

void	game_menu_start_render(Texture2D *texture, Font font) {
	for (int i = 0; i <  BUTTON_NUMBER; i++) {
		haven_gui_button_draw(button[i], texture, font);
	}
}

void	game_menu_start_clear(void) {
	for (int i = 0; i <  BUTTON_NUMBER; i++) {
		haven_gui_button_delete(button[i]);
	}
}
