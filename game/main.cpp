#include <raylib.h>
#include <raymath.h>
#include "main.hpp"

int main(void) {
	InitWindow(720, 480, "Game");

	engine_t engine = {0};
	engine = init_engine();

	level_t level = {0};

	HideCursor();
	level.terrain.model = LoadModelFromMesh(GenMeshHeightmap(LoadImage("assets/heightmap/snowdon.png"), {1000, 100, 1000}));
	level.terrain.scale = 1;
	level.terrain.pos = {0, 0, 0};
	level.terrain.model.materials[0].shader = engine.gbuffer_shader;
	level.terrain.bound = GetModelBoundingBox(level.terrain.model);

	while (!WindowShouldClose()) {
		update_input(engine);
		// engine.player.pos = camera.position;
		render(level, engine, NULL);
	}
	UnloadModel(level.terrain.model);
	ShowCursor();
	void close_engine(engine_t &engine);
}
