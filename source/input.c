#include <engine.h>

void	update_input(engine_t *engine) {
	Vector2 delta = GetMouseDelta();
	SetMousePosition(GetScreenWidth() * 0.5, GetScreenHeight() * 0.5); 
	Vector3 step = {0};

	if (IsKeyDown(KEY_W)) {
		step.x++;
	}
	if (IsKeyDown(KEY_S)) {
		step.x--;
	}
	if (IsKeyDown(KEY_A)) {
		step.y--;
	}
	if (IsKeyDown(KEY_D)) {
		step.y++;
	}
	if (IsKeyDown(KEY_SPACE)) {
		step.z++;
	}
	if (IsKeyDown(KEY_LEFT_SHIFT)) {
		step.z--;
	}
	if (IsKeyPressed(KEY_Q)) {
		int i = engine->mode;
		i++;
		i%= 5;
		engine->mode = (deferred_mode)i; 
	}
	if (IsKeyPressed(KEY_E)) {

	}
	if (IsKeyPressed(KEY_TAB)) {
		engine->player.show_inventory = !engine->player.show_inventory;
	}
	UpdateCameraPro(&engine->camera, step, (Vector3){(float)(delta.x * 0.2), (float)(delta.y * 0.2), 0}, 0);
}
