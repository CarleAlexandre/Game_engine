#include "voxel.h"
#define FNL_IMPL
#include <lib/FastNoiseLite.h>
#include <stdio.h>

voxel_data *get_chunk_voxels(voxel_world *world, int x, int y, int z) {
	return world->voxel_arena + ((x * CHUNK_STRIDE_X + y * CHUNK_STRIDE_Y + z) * CHUNK_SIZE);
}

void	island_mask(Image *input) {
	Image mask = GenImageGradientRadial(1024, 1024, 0.5, BLACK, WHITE);

	Color *mask_colors = LoadImageColors(mask);
	// UnloadImage(mask);

	float gray = 0;

	Color *input_color = LoadImageColors(*input);
	for (int i = 0; i < 1024 * 1024; i++) {
		gray = GRAY_VALUE(input_color[i]) - GRAY_VALUE(mask_colors[i]);
		gray = Clamp(gray, 0, 254);
		input_color[i] = (Color){.a = 255, .b = gray, .g = gray, .r = gray};
		ImageDrawPixel(input, i % 1024, i / 1024, input_color[i]);
	}
	UnloadImageColors(mask_colors);
	UnloadImageColors(input_color);
}

void	rain_mask(Image *input, fnl_state *noise) {
	float mask;

	Color *input_color = LoadImageColors(*input);
	for (int i = 0; i < 1024 * 1024; i++) {
		mask = fnlGetNoise2D(noise, i / 1024, - (i % 1024)) + 1;
		if (mask + GRAY_VALUE(input_color[i]) >= 2) {
			input_color[i].b = 255;
		}
		ImageDrawPixel(input, i % 1024, i / 1024, input_color[i]);
	};
	
	UnloadImageColors(input_color);
}

Texture2D	gen_texture_noise(fnl_state *noise) {
	Image test = GenImageColor(1024, 1024, BLACK);
	for (int x = 0; x < 1024; x++) {
		for (int z = 0; z < 1024; z++) {
			float noise_data = fnlGetNoise2D(noise, x, z) + 1;
			char color_data = floorf(noise_data * 255 / 2);
			Color tmp = {
				.a = 255,
				.r = color_data,
				.g = color_data,
				.b = color_data,
			};
			ImageDrawPixel(&test, x, z, tmp);
		}
	}

	// ImageColorInvert(&test);
	island_mask(&test);
	rain_mask(&test, noise);
	Texture2D text = LoadTextureFromImage(test);
	UnloadImage(test);
	return (text);
}

void	chunk_gen_height(int x_off, int z_off, fnl_state *noise) {
	float value[64][64] = {0};
	float max = 0;
	for (int x = 0; x < 64; x++) {
		for (int z = 0; z < 64; z++) {	
			value[x][z] = (fnlGetNoise2D(noise, x_off + x, z_off + z) + 1) * 32 + 64;
			if (value[x][z] > max) max = value[x][z];
		}
	}
}

voxel_world	*gen_world(uint32_t seed) {
	printf("The Mad God has Begun Creation\n");

	voxel_world *world = malloc(sizeof(voxel_world));
	assert(world);

	// Allocate a single arena for all voxel data
	world->voxel_arena = malloc(sizeof(voxel_data) * CHUNK_SIZE * 32 * 8 * 32);
	assert(world->voxel_arena);

	fnl_state noise = fnlCreateState();
	noise.seed = seed;
	noise.noise_type = FNL_NOISE_OPENSIMPLEX2;
	noise.frequency = 0.02f;
	noise.octaves = 5;

	// Initialize all chunks
	for (int x = 0; x < 32; x++) {
		for (int y = 0; y < 8; y++) {
			for (int z = 0; z < 32; z++) {
				world->chunk[x][y][z] = malloc(sizeof(voxel_chunk));
				assert(world->chunk[x][y][z]);
				
				// Initialize chunk position
				world->chunk[x][y][z]->positon = (Vector3){
					x * CHUNK_RESOLUTION,
					y * CHUNK_RESOLUTION, 
					z * CHUNK_RESOLUTION
				};

				// Generate terrain for this chunk
				chunk_gen_height(x * CHUNK_RESOLUTION, z * CHUNK_RESOLUTION, &noise);

				// Get this chunk's voxel data
				voxel_data *chunk_voxels = get_chunk_voxels(world, x, y, z);
			}
		}
	}

	printf("World Generation Complete\n");

	return (world);
}
