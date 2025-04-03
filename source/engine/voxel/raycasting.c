#include "voxel.h"
#include <xmmintrin.h>

void	ray_init() {

}


void	voxel_raycasting(const Ray ray, octree_node* node) {
	BoundingBox box;
	
	__m128 center;
	center = _mm_load_ps(node->center);

	__m128 extent;
	extent = _mm_load_ps1(&node->extent);
	
	// Calculate min and max vectors in one SIMD operation each
	__m128 min_vec = _mm_sub_ps(center, extent);
	__m128 max_vec = _mm_add_ps(center, extent);
	
	// Extract components
	box.min = (Vector3){
		.x = _mm_cvtss_f32(min_vec),
		.y = _mm_cvtss_f32(_mm_shuffle_ps(min_vec, min_vec, _MM_SHUFFLE(1, 1, 1, 1))),
		.z = _mm_cvtss_f32(_mm_shuffle_ps(min_vec, min_vec, _MM_SHUFFLE(2, 2, 2, 2)))
	};
	box.max = (Vector3){
		.x = _mm_cvtss_f32(max_vec),
		.y = _mm_cvtss_f32(_mm_shuffle_ps(max_vec, max_vec, _MM_SHUFFLE(1, 1, 1, 1))),
		.z = _mm_cvtss_f32(_mm_shuffle_ps(max_vec, max_vec, _MM_SHUFFLE(2, 2, 2, 2)))
	};

	RayCollision collision = GetRayCollisionBox(ray, box);
	// If we hit the box, check children or return hit if leaf node
	if (collision.hit) {
		// If this is a leaf node with data, we found our voxel
		if (node->data) {
			return;
		}

		// Check all child nodes that exist
		for (int i = 0; i < 8; i++) {
			if (octree_node_isleaf(node, i) && node->children[i]) {
				voxel_raycasting(ray, node->children[i]);
			}
		}
	}
}