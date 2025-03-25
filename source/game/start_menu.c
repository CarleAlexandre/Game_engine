#include "game.h"
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

int	game_menu_start(void) {
	if (GuiButton((Rectangle){20, 100, 200, 50}, "Offline")) {
		return (1);
	}
	if (GuiButton((Rectangle){20, 200, 200, 50}, "Online")) {
		return (2);
	}
	if (GuiButton((Rectangle){20, 300, 200, 50}, "setting")) {
		return (3);
	}
	if (GuiButton((Rectangle){20, 400, 200, 50}, "quit")) {
		return (4);
	}
	return (0);
}
