#include <prototype.h>

void	update_input(engine_t *engine, world_t *world) {
	Vector2 delta = GetMouseDelta();
	SetMousePosition(GetScreenWidth() * 0.5, GetScreenHeight() * 0.5); 
	Vector3 step = {0};

	if (IsKeyDown(KEY_W)) {
		step.x += 30 *GetFrameTime();
	}
	if (IsKeyDown(KEY_S)) {
		step.x -= 30 *GetFrameTime();
	}
	if (IsKeyDown(KEY_A)) {
		step.y -= 30 *GetFrameTime();
	}
	if (IsKeyDown(KEY_D)) {
		step.y += 30 *GetFrameTime();
	}
	if (IsKeyDown(KEY_SPACE)) {
		step.z += 30 *GetFrameTime();
	}
	if (IsKeyDown(KEY_LEFT_SHIFT)) {
		step.z -= 30 *GetFrameTime();
	}
	if (IsKeyPressed(KEY_Q)) {
	}
	if (IsKeyPressed(KEY_E)) {
	}
	if (IsKeyPressed(KEY_TAB)) {
		engine->player.show_inventory = !engine->player.show_inventory;
	}
	UpdateCameraPro(&engine->camera, step, (Vector3){(float)(delta.x * 0.2), (float)(delta.y * 0.2), 0}, 0);
	if (step.x != 0 || step.y != 0 || step.z != 0 || delta.x != 0 || delta.y != 0) {
		update_world_render(world, engine);
	}
}
