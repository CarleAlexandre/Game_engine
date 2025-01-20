#include <engine.hpp>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

void draw_inventory(inventory_t inventory) {
	//row = 6 item,
	Rectangle pos = {20, 20, 650, 400};
	Rectangle bound = {0, 0, 600, (float)100 * (inventory.size() / 6)};
	Rectangle view;
	Vector2 slider;

	GuiScrollPanel(pos, "inventory", bound, &slider, &view);
	BeginScissorMode(view.x + slider.x, view.y + slider.y, view.width, view.height);
	EndScissorMode();
}

void draw_toolbar(tool_bar_t *tool_bar) {
	static Vector2 pos = {GetScreenWidth() - 140, GetScreenHeight() - 100};
	static Vector2 pos1 = {pos.x + 20, pos.y + 10}, topos1 = pos1;
	static Vector2 pos2 = {pos.x + 80, pos.y + 10}, topos2 = pos2;
	static bool gotoprev = false;
	static bool gotonext = false;

	if (gotonext || gotoprev) {
		pos1 = Vector2MoveTowards(pos1, topos1, 10);
		pos2 = Vector2MoveTowards(pos2, topos2, 10);
		if (abs(pos1.x - topos1.x) <= 0.1 || abs(pos2.x - topos2.x) <= 0.1) {
			gotonext = false;
			gotoprev = false;
		}
	}
	if (tool_bar->gotoprev) {
		topos1.x = pos.x - 40;
		topos1.y = pos.y + 10;
		topos2.x = pos.x + 20;
		topos2.y = pos.y + 10;
		pos1 = {pos.x + 20, pos.y + 10};
		pos2 = {pos.x + 80, pos.y + 10};
		gotoprev = true;
		gotonext = false;
		tool_bar->current_item--;
		tool_bar->gotoprev = false;
	}
	if (tool_bar->gotonext) {
		topos1.x = pos.x + 20;
		topos1.y = pos.y + 10;
		topos2.x = pos.x + 80;
		topos2.y = pos.y + 10;
		pos1 = {pos.x - 40, pos.y + 10};
		pos2 = {pos.x + 20, pos.y + 10};
		gotonext = true;
		gotoprev = false;
		tool_bar->current_item++;
		tool_bar->gotonext = false;
	}
	BeginScissorMode(pos.x, pos.y, 80, 60);
	// DrawTexturePro(, {}, {0, 0, 40, 40}, pos1, 0, WHITE);
	// DrawTexturePro(, {}, {0, 0, 40, 40}, pos2, 0, WHITE);
	DrawRectangleRec({pos1.x, pos1.y,40,40}, WHITE);
	DrawRectangleRec({pos2.x, pos2.y,40,40}, WHITE);
	EndScissorMode();
}

void draw_ui(std::vector<Texture2D> texture, int TEXTURE_CROSSAIR, sv_player_t player) {
	if (player.stats.health < player.stats.max_health) {
		DrawRectangle(30 + player.stats.health, 100, (player.stats.max_health - player.stats.health), 10, GRAY);
	}
	DrawRectangle(30, 100, player.stats.health, 10, RED);
	DrawRectangle(GetScreenWidth() / 2 - 1, GetScreenHeight() / 2 - 1, 2, 2, RED);
	//DrawTexture(texture[TEXTURE_CROSSAIR], GetScreenWidth(), GetScreenHeight(), WHITE);
	draw_toolbar(&player.toolbar);
	if (player.show_inventory) {
		draw_inventory(player.inventory);
	}
}
