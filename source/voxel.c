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

void	add_map_obj() {

}

void	add_block() {

}

chunk_t *generate_terrain(Vector2 chunk_pos) {
	// Create and configure noise state
	fnl_state noise = fnlCreateState();
	noise.noise_type = FNL_NOISE_OPENSIMPLEX2S;

	chunk_t *chunk = malloc(sizeof(chunk_t));
	memset(chunk, 0, sizeof(chunk_t));

	for (int x = 0; x < 32; x++) {
		for (int z = 0; z < 32; z++) {
			float noise_data = fnlGetNoise2D(&noise, x + chunk_pos.x *32 , z + chunk_pos.y * 32) * 2;
			for (int y = 0; y < noise_data; y++) {
				chunk->blocks[x][z][y] = true;
			}
		}
	}
	return (chunk);
}

void	generate_dungeon() {
	
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

void	generate_chunk_mesh(engine_t *engine, chunk_t *chunk[5][5]) {
	for (int h = 0; h < 5; h++) {
		for (int l = 0; l < 5; l++) {
			for (int x = 0; x < 32; x++) {
				for (int z = 0; z < 32; z++) {
					for (int y = 0; y < 32; y++) {
						if (chunk[h][l]->blocks[x][z][y]) {
							//if empty don't add face;
							DrawModel(engine->cube, (Vector3){x + h * 32, y, z + l * 32}, 1, WHITE);

						// // if no vox above
						// if (y < 31 && !chunk->blocks[x][z][y + 1]) {
						// }
						// //add_top_face();
						
						// // if no vox below
						// if (y > 0 && !chunk->blocks[x][z][y - 1]) {

						// }
						// //add_bottom_face();

						// // if no vox on left
						// if (z < 31 && !chunk->blocks[x + 1][z][y]) {

						// }
						// //add_left_face();
						
						// // if no vox on right
						// if (z > 0 && !chunk->blocks[x - 1][z][y]) {

						// }
						// //add_right_face();
						
						// // if no vox behind
						// if (x < 31 && !chunk->blocks[x + 1][z][y]) {

						// }
						// //add_back_face();
						
						// // if no vox in front
						// if (x > 0 && !chunk->blocks[x - 1][z][y]) {

						// }
						// //add_front_face();
						}
					}
				}
			}
		}
	}
}

void	combine_chunk_data(){

}

void	draw_chunks() {
	// glDrawArraysInstancedBaseInstance();
}
