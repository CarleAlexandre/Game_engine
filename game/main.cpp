#include <raylib.h>
#include <raymath.h>
#include "main.hpp"

int main(void) {
	engine_t engine;

	InitWindow(720, 480, "Game");

	engine = init_engine();

	level_t level;

	//create_light(level.lights, {10, 10, 10}, GLOBAL_LIGHT, 1, engine.light);

	while (!WindowShouldClose()) {
	//	update_light(level.lights, engine.light);
		render(level, engine, NULL);
	}
	void close_engine(engine_t &engine);
}
