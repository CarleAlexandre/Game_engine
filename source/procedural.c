#define FNL_IMPL
#include <engine.h>

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
