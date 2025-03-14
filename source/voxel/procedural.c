#define FNL_IMPL
#include <engine.h>
#include <stdio.h>

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

chunk_t	**chunk_gen_height(int x_off, int z_off, int *size, fnl_state *noise) {
	chunk_t **slice;

	float value[64][64] = {0};
	float max = 0;
	for (int x = 0; x < 64; x++) {
		for (int z = 0; z < 64; z++) {	
			value[x][z] = (fnlGetNoise2D(noise, x_off + x, z_off + z) + 1) * 32 + 64;
			if (value[x][z] > max) max = value[x][z];
		}
	}

	(*size) = floorf(max / 64) + 1;
	if ((*size) > 8) *size = 8;
	slice = malloc(sizeof(chunk_t*) * (*size));
	for (int i = 0; i < (*size); i++) {
		slice[i] = malloc(sizeof(chunk_t));
		slice[i]->blocks =  init_svo(64, 6);
		slice[i]->pos = (Vector3){x_off * 0.5, i * 32, z_off * 0.5};
	}
	for (int x = 0; x < 64; x++) {
		for (int z = 0; z < 64; z++) {	
			for (int y = 0; y < value[x][z]; y++) {
				voxel_t *vox = malloc(sizeof(voxel_t));
				vox->block_id = 1;
				if (y < 64) {
					svo_insert((Vector3){x, y, z}, vox, slice[0]->blocks);
				} else if (y < 128) {
					svo_insert((Vector3){x, y - 64, z}, vox, slice[1]->blocks);
				} else if (y < 196) {
					svo_insert((Vector3){x, y - 128, z}, vox, slice[2]->blocks);
				} else if (y < 256) {
					svo_insert((Vector3){x, y - 196, z}, vox, slice[3]->blocks);
				} else if (y < 320) {
					svo_insert((Vector3){x, y - 256, z}, vox, slice[4]->blocks);
				} else if (y < 384) {
					svo_insert((Vector3){x, y - 320, z}, vox, slice[5]->blocks);
				} else if (y < 448) {
					svo_insert((Vector3){x, y - 384, z}, vox, slice[6]->blocks);
				} else {
					svo_insert((Vector3){x, y - 448, z}, vox, slice[7]->blocks);
				}
			}
		}
	}

	return (slice);
}

void	gen_world() {
	printf("Uruk The Mad God has Begun Creation\n");
}
