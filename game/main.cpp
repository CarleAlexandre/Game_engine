#include <raylib.h>
#include <raymath.h>
#include "main.hpp"
#include <iostream>

int main(void) {


	InitWindow(720, 480, "Game");

	engine_t engine = {0};
	engine = init_engine();
	level_t level = {0};

	//load all item,
	//load all obj,
	//Load all texture,
	//Load all model,
	//Load all osund,
	//Load all other asset

	HideCursor();
	level.terrain.model = LoadModelFromMesh(GenMeshHeightmap(LoadImage("assets/heightmap/snowdon.png"), {1000, 100, 1000}));
	level.terrain.scale = 1;
	level.terrain.pos = {0, 0, 0};
	level.terrain.model.materials[0].shader = engine.gbuffer_shader;
	level.terrain.bound = GetModelBoundingBox(level.terrain.model);

	engine.player.stats.max_health = 150;
	engine.player.stats.health = 100;

	double launch_time = GetTime();

	while (!WindowShouldClose()) {
		update_input(engine);
		// engine.player.pos = camera.position;
		render(level, engine, NULL);
	}
	UnloadModel(level.terrain.model);
	ShowCursor();
	void close_engine(engine_t &engine);
	std::cout << "INFO: time to launch : " << launch_time * 1000 << " ms\n";
	std::cout << "INFO: time to close : " <<  GetTime() << " s\n";
}
