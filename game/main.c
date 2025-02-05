#include <raylib.h>
#include <raymath.h>
#include <prototype.h>
#include <stdio.h>

int main(void) {
	InitWindow(1920, 1080, "Game");

	engine_t engine = {0};
	engine = init_engine();

	//load all item,
	//load all obj,
	//Load all texture,
	//Load all model,
	//Load all sound,

	HideCursor();

	chunk_t *world[5][5];
	for (int x = 0; x < 5; x++) {
		for (int z = 0; z < 5; z++) {
			world[x][z] = generate_terrain((Vector2){x, z});
		}
	}

	engine.lights[0] = CreateLight(LIGHT_POINT, (Vector3){ -0, 40, -100 }, Vector3Zero(), YELLOW, engine.deffered_shader);
	engine.lights[1] = CreateLight(LIGHT_POINT, (Vector3){ 0, 40, 100 }, Vector3Zero(), RED, engine.deffered_shader);
	engine.lights[2] = CreateLight(LIGHT_POINT, (Vector3){ -100, 40, 0 }, Vector3Zero(), GREEN, engine.deffered_shader);
	engine.lights[3] = CreateLight(LIGHT_POINT, (Vector3){ 100, 40, -0 }, Vector3Zero(), BLUE, engine.deffered_shader);

	engine.cube = LoadModelFromMesh(GenMeshCube(1, 1, 1));
	engine.Sphere = LoadModelFromMesh(GenMeshSphere(5, 10, 10));
	engine.dummy = LoadModelFromMesh(GenMeshHeightmap(LoadImage("assets/heightmap/snowdon.png"), (Vector3){100, 10, 100}));
	engine.cube.materials[0].shader = engine.gbuffer_shader;
	engine.Sphere.materials[0].shader = engine.gbuffer_shader;
	engine.dummy.materials[0].shader = engine.gbuffer_shader;

	engine.player.stats.max_health = 150;
	engine.player.stats.health = 100;
	engine.player.bound = (BoundingBox){
		.min = {-0.5, 0, -0.5},
		.max = {0.5, 2, 0.5}
	};
	init_toolbar(&engine.player.toolbar);

	double launch_time = GetTime();

	SetTargetFPS(120);
	while (!WindowShouldClose()) {
		update_input(&engine);
		// update(&input);
		engine.player.pos = engine.camera.position;
		render(&engine, world);
	}
	ShowCursor();
	close_engine(&engine);
	printf("INFO: time to launch : %.2lfms\n", launch_time * 1000);
	printf("INFO: time to close : %.2lfs\n", GetTime());
	return (0);
}
