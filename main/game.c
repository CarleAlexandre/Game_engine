#include <prototype.h>
#include <stdio.h>

void loadCubemap(void) {
	const char *skybox[6] = {
		"include/ext/skybox1/posx.png",
		"include/ext/skybox1/negx.png",
		"include/ext/skybox1/posy.png",
		"include/ext/skybox1/negy.png",
		"include/ext/skybox1/posz.png",
		"include/ext/skybox1/negz.png"
	};

	Image cubemap = GenImageColor(2048, 12288, BLACK);
	for (unsigned int i = 0; i < 6; i++) {
		Image img = LoadImage(skybox[i]);
		Color *color = LoadImageColors(img);
		for (int y = 0; y < 2048; y++) {
			for (int x = 0; x < 2048; x++) {
				ImageDrawPixel(&cubemap, x, y + 2048 * i, color[x + y * 2048]);
			}
		}
		UnloadImageColors(color);
		UnloadImage(img);
	}
	ExportImage(cubemap, "include/ext/skybox.png");
}

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

	world->chunks = init_svo(8, 3);
	int size = 0;
	chunk_t **tmp;
	fnl_state noise = fnlCreateState();

	noise.frequency = 0.003;
	noise.fractal_type = FNL_FRACTAL_FBM;
	noise.gain = 0.8;

	// loadCubemap();

	// goto end;

	double time = GetTime();

	for (int x = 0; x < 8; x++) {
		for (int z = 0; z < 8; z++) {
			tmp = chunk_gen_height(x * 64, z * 64, &size, &noise);
			for (int y = 0; y < size; y++) {
				tmp[y]->pos = (Vector3){
					x * 32.0f,
					y * 32.0f,
					z * 32.0f
				};
				svo_insert((Vector3){x, y, z}, tmp[y], world->chunks);
			}
			tmp = 0x00;
			size = 0;
		}
	}
	world->rcount = 0;

	printf("INFO: world Creation : %.2lfs\n", GetTime() - time);
	time = GetTime();
	
	gen_world_mesh(world, &engine);

	printf("INFO: world meshing : %.2lfs\n", GetTime() - time);
	time = GetTime();

	setup_world_render(world, &engine);
	for (int i = 0; i < world->rcount; i++) {
		gen_chunk_render(world->rqueue[i]->mesh);
	}

	printf("INFO: world render : %.2lfs\n", GetTime() - time);


	engine.player.stats.max_health = 150;
	engine.player.stats.health = 100;
	engine.player.bound = (BoundingBox){
		.min = {-0.5, 0, -0.5},
		.max = {0.5, 2, 0.5}
	};
	init_toolbar(&engine.player.toolbar);

	double launch_time = GetTime();
	
	SetTargetFPS(165);
	while (!WindowShouldClose()) {
		update_input(&engine, world);
		voxel_render(&engine, world);
	}

	end:
	
	ShowCursor();
	free(world);
	printf("INFO: time to launch : %.2lfms\n", launch_time * 1000);
	printf("INFO: total run time : %.2lfs\n", GetTime());
	close_engine(&engine);
	return (0);
}
