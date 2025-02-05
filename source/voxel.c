#include <engine.h>
#include <stdio.h>
#include <string.h>
#define FNL_IMPL
#include <FastNoiseLite.h>

// x = index / (size * size)
// y = (index / size) % size
// z = index % size

// void heightmap_voxel(Image heightmap) {
// 	const int mapX = heightmap.width;
// 	const int mapZ = heightmap.height;

// 	Color *pixels = LoadImageColors(heightmap);
// 	chunk_t *chunk;

// 	int size = (heightmap.width - (heightmap.width % 16)) * (heightmap.height - (heightmap.height % 16));

// 	int alloc_size = 16 * 16 * 16 * sizeof(int);
// 	printf("%ix%i : %i :: %i\n", heightmap.height, heightmap.width, size, (alloc_size * size * size));

// 	for (int i = 0; i < size; i++) {
// 		chunk[i].blocks = (int *)malloc(alloc_size * size * size);
// 		memset(chunk[i].blocks, 0, alloc_size);
// 		for (int z = 0; z < 16; z++) {
// 			for (int x = 0; x < 16; x++) {
// 				for (int y = 0;y < GRAY_VALUE(pixels[x + (z + 1)*mapX]) && y < 16; y++) {
// 					chunk[i].blocks[z * 16 * 16 + x * 16 + y] = 1;
// 				}
// 			}
// 		}
// 	}

// 	// return (chunk);
// }

void add_map_obj() {

}

void add_block() {

}

chunk_t *generate_terrain() {
	// Create and configure noise state
	fnl_state noise = fnlCreateState();
	noise.noise_type = FNL_NOISE_OPENSIMPLEX2;

	chunk_t *chunk = malloc(sizeof(chunk_t));

	for (int x = 0; x < 32; x++) {
		for (int z = 0; z < 32; z++) {
			for (int y = 0; y < fnlGetNoise2D(&noise, x, z); y++) {
				chunk->blocks[x][z][y] = true;
			}
		}
	}
	return (chunk);
}

void generate_dungeon() {
	
}

void	load_chunk_data() {

}

void	unload_chunk_data() {

}

//voxel vertice : 

Mesh	add_face(face_orientation_e face, Vector3 pos) {
	switch (face){
		case(top_face): {
			
		}
		case(bot_face): {

		}
		case(left_face): {

		}
		case(right_face): {

		}
		case(front_face): {

		}
		case(back_face): {

		}
		default:break;
	}
}

void	generate_chunk_mesh(chunk_t *chunk) {
	for (int x = 0; x < 32; x++) {
		for (int z = 0; z < 32; z++) {
			for (int y = 0; y < 32; y++) {
				chunk->blocks[x][z][y];
				//if empty don't add face;

				// if no vox above
				if (y < 31 && !chunk->blocks[x][z][y + 1]) {
				}
				//add_top_face();
				
				// if no vox below
				if (y > 0 && !chunk->blocks[x][z][y - 1]) {

				}
				//add_bottom_face();

				// if no vox on left
				if (z < 31 && !chunk->blocks[x + 1][z][y]) {

				}
				//add_left_face();
				
				// if no vox on right
				if (z > 0 && !chunk->blocks[x - 1][z][y]) {

				}
				//add_right_face();
				
				// if no vox behind
				if (x < 31 && !chunk->blocks[x + 1][z][y]) {

				}
				//add_back_face();
				
				// if no vox in front
				if (x > 0 && !chunk->blocks[x - 1][z][y]) {

				}
				//add_front_face();
			}
		}
	}
}

void	combine_chunk_data(){

}

void	draw_chunks() {
	// glDrawArraysInstancedBaseInstance();
}
