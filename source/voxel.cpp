#include <engine.hpp>
#include <vector>

#define GRAY_VALUE(c) ((float)(c.r + c.g + c.b)/3.0f)

void lod_voxel(Vector3 pos_player, Vector3 pos_obj) {
	float tmp = Vector3Distance(pos_obj, pos_player);
	if (tmp < 32) {

	}
	else {

	}
}

void renderface(){}

void heightmap_voxel(Image heightmap) {
	const int mapX = heightmap.width;
	const int mapZ = heightmap.height;

	Color *pixels = LoadImageColors(heightmap);
	std::vector<std::vector<int>> height = std::vector<std::vector<int>>(mapZ);
	for (auto tmp : height) {
		tmp = std::vector<int>(mapX);
	}

	for (int z = 0; z < mapZ - 1; z++) {
		for (int x = 0; x < mapX - 1; x++) {
			height[z][x] = GRAY_VALUE(pixels[x + (z + 1)*mapX]);
		}
	}

	
}

void add_map_obj() {
	 
}

void raycaster(Camera3D camera, level_t level) {
	int width = GetScreenWidth(), height = GetScreenHeight();
	for (float y = 0; y < height; y++) {
		for (float x = 0; x < width; x++) {
			Ray ray = GetScreenToWorldRay({x, y}, camera);
			for (auto &span : level.objs) {
				RayCollision collision = GetRayCollisionBox(ray, span.bound);
				if (collision.hit) {
					span.render = true;
					break;
				}
			}
		}
	}
}
