#include <raylib.h>
#include <raymath.h>
#include "main.hpp"

int main(void) {
	InitWindow(720, 480, "Game");

	engine_t engine = {0};
	engine = init_engine();

	level_t level = {0};

	HideCursor();
	level.terrain.model = LoadModelFromMesh(GenMeshPlane(100, 100, 100, 100));
	level.terrain.scale = 1;
	level.terrain.pos = {-10, -5, -10};
	level.terrain.model.materials[0].shader = engine.gbuffer_shader;

	while (!WindowShouldClose()) {
		update_input(engine);
		// engine.player.pos = camera.position;
		render(level, engine, NULL);
	}
	UnloadModel(level.terrain.model);
	ShowCursor();
	void close_engine(engine_t &engine);
}
