#include <engine.hpp>
#include <vector>

int **heightmap_voxel(Image heightmap) {
	const int mapX = heightmap.width;
	const int mapZ = heightmap.height;

	Color *pixels = LoadImageColors(heightmap);
	int **data;
	data = (int **)malloc(sizeof(int) * mapX * mapZ);

	for (int z = 0; z < mapZ - 1; z++) {
		for (int x = 0; x < mapX - 1; x++) {
			*data[z * mapZ + x] = GRAY_VALUE(pixels[x + (z + 1)*mapX]);
		}
	}
	return (data);
}

void add_map_obj() {

}

void generate_dungeon() {
	
}


