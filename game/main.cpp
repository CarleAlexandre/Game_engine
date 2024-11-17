#include <raylib.h>
#include <raymath.h>
#include "main.hpp"

int main(void) {
	engine_t engine;

	InitWindow(720, 480, "Game");

	engine = init_engine();

	level_t level;

	//create_light(level.lights, {10, 10, 10}, GLOBAL_LIGHT, 1, engine.light);

	HideCursor();
	level.terrain.model = LoadModelFromMesh(GenMeshPlane(100, 100, 100, 100));
	level.terrain.scale = 1;
	level.terrain.pos = {-10, -5, -10};
	level.terrain.model.materials[0].shader = engine.gbuffer_shader;


	while (!WindowShouldClose()) {
		update_input(engine);
	//	update_light(level.lights, engine.light); 
		render(level, engine, NULL);
	}
	UnloadModel(level.terrain.model);
	ShowCursor();
	void close_engine(engine_t &engine);
}
