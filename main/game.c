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

	world_t *world = malloc(sizeof(world_t));

	world->tree = init_svo(4, 2);
	int size;
	chunk_t **tmp;
	fnl_state noise = fnlCreateState();

	for (int x = 0; x < 4; x++) {
		for (int z = 0; z < 4; z++) {
			tmp = chunk_gen_height(x * 64, z * 64, &size, &noise);
			for (int y = 0; y < size ; y++) {
				tmp[y]->pos = (Vector3){
					x * 64.0f,
					y * 64.0f,
					z * 64.0f
				};
				svo_insert((Vector3){x, y, z}, tmp[y], world->tree);
			}
			tmp = 0x00;
			size = 0;
		}
	}
	world->rcount = 0;

	gen_world_mesh(world, &engine);
	update_world_render(world, &engine);
	
	for (int i = 0; i < world->rcount; i++) {
		gen_chunk_render(world->rqueue[i]->mesh);
	}

	// goto end;

	engine.player.stats.max_health = 150;
	engine.player.stats.health = 100;
	engine.player.bound = (BoundingBox){
		.min = {-0.5, 0, -0.5},
		.max = {0.5, 2, 0.5}
	};
	init_toolbar(&engine.player.toolbar);

	double launch_time = GetTime();
	
	SetTargetFPS(240);
	while (!WindowShouldClose()) {
		update_input(&engine, world);
		voxel_render(&engine, world);
	}

	end:

	ShowCursor();
	free(world);
	close_engine(&engine);
	printf("INFO: time to launch : %.2lfms\n", launch_time * 1000);
	printf("INFO: time to close : %.2lfs\n", GetTime());
	return (0);
}
