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

void draw_ui(std::vector<Texture2D> texture, int TEXTURE_CROSSAIR, sv_player_t player) {
	if (player.stats.health < player.stats.max_health) {
		DrawRectangle(30 + player.stats.health, 100, (player.stats.max_health - player.stats.health), 10, GRAY);
	}
	DrawRectangle(30, 100, player.stats.health, 10, RED);
	DrawRectangle(GetScreenWidth() / 2 - 1, GetScreenHeight() / 2 - 1, 2, 2, RED);
	//DrawTexture(texture[TEXTURE_CROSSAIR], GetScreenWidth(), GetScreenHeight(), WHITE);
}
