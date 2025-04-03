#include "voxel.h"

void	ray_init() {

}

bool	voxel_node_collision(const Ray ray, octree_node* node) {
	const BoundingBox box = {
		.min = {
			.x = node->center[0] - node->extent,
			.y = node->center[1] - node->extent,
			.z = node->center[2] - node->extent
		},
		.max = {
			.x = node->center[0] + node->extent,
			.y = node->center[1] + node->extent,
			.z = node->center[2] + node->extent
		}
	};
	RayCollision collision = GetRayCollisionBox(ray, box);
	if (collision.hit) {
		if (node->data) {
			return (false);
		}
		for (int i = 0; i < 8; i++) {
			if (octree_node_isleaf(node, i) && node->children[i]) {
				if (voxel_raycasting(ray, node->children[i])) {
					return (true);
				}
			}
		}
	}
}