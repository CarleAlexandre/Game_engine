#include <engine.hpp>

void lod_voxel() {
	
}

void heightmap_voxel() {

}

void add_map_obj() {
}

void raycaster(Camera3D camera, level_t level) {
	int width = GetScreenWidth(), height = GetScreenHeight();
	for (float y = 0; y < height; y++) {
		for (float x = 0; x < width; x++) {
			Ray ray = GetScreenToWorldRay({x, y}, camera);
			for (auto &span : level.objs) {
				if (GetRayCollisionBox(ray, span.bound).hit) {
					span.render = true;
				}
			}
		}
	}
}
