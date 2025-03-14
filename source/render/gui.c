#include <raygui.h>

// void	draw_inventory(inventory_t inventory) {
// 	//row = 6 item,
// 	Rectangle pos = {20, 20, 650, 400};
// 	Rectangle bound = {0, 0, 600, (float)100 * (inventory.size / 6)};
// 	Rectangle view;
// 	Vector2 slider;

// 	GuiScrollPanel(pos, "inventory", bound, &slider, &view);
// 	BeginScissorMode(view.x + slider.x, view.y + slider.y, view.width, view.height);
// 	EndScissorMode();
// }

// void	init_toolbar(tool_bar_t *tool_bar) {
// 	tool_bar->pos1 = (Vector2){GetScreenWidth() - 140 + 20, GetScreenHeight() - 100 + 10};
// 	tool_bar->pos2 = (Vector2){GetScreenWidth() - 140 + 80, GetScreenHeight() - 100 + 10};
// 	tool_bar->topos1 = tool_bar->pos1;
// 	tool_bar->topos2 = tool_bar->pos2;
// }

// void	draw_toolbar(tool_bar_t *tool_bar) {
// 	Vector2 pos = (Vector2){GetScreenWidth() - 140, GetScreenHeight() - 100};
// 	static bool gotoprev = false;
// 	static bool gotonext = false;

// 	if (gotonext || gotoprev) {
// 		tool_bar->pos1 = Vector2MoveTowards(tool_bar->pos1, tool_bar->topos1, 10);
// 		tool_bar->pos2 = Vector2MoveTowards(tool_bar->pos2, tool_bar->topos2, 10);
// 		if (abs(tool_bar->pos1.x - tool_bar->topos1.x) <= 0.1 || abs(tool_bar->pos2.x - tool_bar->topos2.x) <= 0.1) {
// 			gotonext = false;
// 			gotoprev = false;
// 		}
// 	}
// 	if (tool_bar->gotoprev) {
// 		tool_bar->topos1.x = pos.x - 40;
// 		tool_bar->topos1.y = pos.y + 10;
// 		tool_bar->topos2.x = pos.x + 20;
// 		tool_bar->topos2.y = pos.y + 10;
// 		tool_bar->pos1 = (Vector2){pos.x + 20, pos.y + 10};
// 		tool_bar->pos2 = (Vector2){pos.x + 80, pos.y + 10};
// 		gotoprev = true;
// 		gotonext = false;
// 		tool_bar->current_item--;
// 		tool_bar->gotoprev = false;
// 	}
// 	if (tool_bar->gotonext) {
// 		tool_bar->topos1.x = pos.x + 20;
// 		tool_bar->topos1.y = pos.y + 10;
// 		tool_bar->topos2.x = pos.x + 80;
// 		tool_bar->topos2.y = pos.y + 10;
// 		tool_bar->pos1 = (Vector2){pos.x - 40, pos.y + 10};
// 		tool_bar->pos2 = (Vector2){pos.x + 20, pos.y + 10};
// 		gotonext = true;
// 		gotoprev = false;
// 		tool_bar->current_item++;
// 		tool_bar->gotonext = false;
// 	}
// 	BeginScissorMode(pos.x, pos.y, 80, 60);
// 	// DrawTexturePro(, {}, {0, 0, 40, 40}, pos1, 0, WHITE);
// 	// DrawTexturePro(, {}, {0, 0, 40, 40}, pos2, 0, WHITE);
// 	DrawRectangleRec((Rectangle){tool_bar->pos1.x, tool_bar->pos1.y,40,40}, WHITE);
// 	DrawRectangleRec((Rectangle){tool_bar->pos2.x, tool_bar->pos2.y,40,40}, WHITE);
// 	EndScissorMode();
// }

// void	draw_ui(sv_player_t player) {
// 	if (player.stats.health < player.stats.max_health) {
// 		DrawRectangle(30 + player.stats.health, 100, (player.stats.max_health - player.stats.health), 10, GRAY);
// 	}
// 	DrawRectangle(30, 100, player.stats.health, 10, RED);
// 	DrawRectangle(GetScreenWidth() / 2 - 1, GetScreenHeight() / 2 - 1, 4, 4, BLACK);
// 	DrawRectangle(GetScreenWidth() / 2 - 1, GetScreenHeight() / 2 - 1, 2, 2, WHITE);
// 	// DrawTexture(texture[TEXTURE_CROSSAIR], GetScreenWidth(), GetScreenHeight(), WHITE);
// 	draw_toolbar(&player.toolbar);
// 	if (player.show_inventory) {
// 		draw_inventory(player.inventory);
// 	}
// 	DrawFPS(10, 10);
// }

void	main_menu() {
	//Solo;
	//Multiplayer
	//Setting
	//leave
}

void	setting_menu() {
	//ui
	//controle
	//video
	//gameplay
	//back
}
