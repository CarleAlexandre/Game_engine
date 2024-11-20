#include <engine.hpp>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

void draw_inventory(inventory_t inventory) {
	//row = 6 item,
	Rectangle pos;
	Rectangle bound;
	Rectangle view;
	Vector2 slider;
	bound = {0, 0, 600, (float)100 * (inventory.size() / 6)};
	pos = {20, 20, 650, 400};

	GuiScrollPanel(pos, "inventory", bound, &slider, &view);
	BeginScissorMode(view.x + slider.x, view.y + slider.y, view.width, view.height);
	EndScissorMode();
}
