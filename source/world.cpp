#include <engine.hpp>
#include <vector>
#include <stdio.h>
#include <string.h>

// x = index / (size * size)
// y = (index / size) % size
// z = index % size

void heightmap_voxel(Image heightmap) {
	const int mapX = heightmap.width;
	const int mapZ = heightmap.height;

	Color *pixels = LoadImageColors(heightmap);
	chunk_t *chunk;

	int size = (heightmap.width - (heightmap.width % 16)) * (heightmap.height - (heightmap.height % 16));

	int alloc_size = 16 * 16 * 16 * sizeof(int);
	printf("%ix%i : %i :: %i\n", heightmap.height, heightmap.width, size, (alloc_size * size * size));

	for (int i = 0; i < size; i++) {
		chunk[i].blocks = (int *)malloc(alloc_size * size * size);
		memset(chunk[i].blocks, 0, alloc_size);
		for (int z = 0; z < 16; z++) {
			for (int x = 0; x < 16; x++) {
				for (int y = 0;y < GRAY_VALUE(pixels[x + (z + 1)*mapX]) && y < 16; y++) {
					chunk[i].blocks[z * 16 * 16 + x * 16 + y] = 1;
				}
			}
		}
	}

	// return (chunk);
}

void add_map_obj() {

}

void add_block() {

}

void generate_dungeon() {
	
}
