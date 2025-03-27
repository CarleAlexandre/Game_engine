#include <stdbool.h>
#include <raylib.h>
#include <raygui.h>

bool	haven_gui_button(const Texture2D texture, const Rectangle bound, const Vector2 mouse_pos) {
	const Rectangle rec = {0, 0, bound.width, bound.height};
	Vector2 pos = {bound.x, bound.y};
	DrawTextureRec(texture, rec, pos, WHITE);
	if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse_pos, bound)) {
		return (true);
	}
	return (false);
}
