#include "voxel_impl.h"

/*
	instead of a mesh use 6 texture with each pixel being a voxel, with depth and if to have less data and better chunk_update
	but can only be used for distant chunk were the player only see a part of it and is not inside the chunk
*/

void	haven_mesh_greedy(chunk_t *current, world_t *world) {
	chunk_builder_data_t chk_build = {0};//need to be mutexed or duplicated
	
	Vector3 pos = {current->pos.x / 32, current->pos.y / 32, current->pos.z / 32};
	// svo_node_t *node = svo_get_node(pos, world->chunks);
	// if (!is_node_valid(node)) return;
	// get_block_data(&chk_build, ((chunk_t *)node->data)->blocks);
	for (int i = 0; i < 6; i++) {
		current->mesh->faces_count[i] = 0;
		pos = (Vector3){current->pos.x / 32, current->pos.y / 32, current->pos.z / 32};
		switch(i) {
			case XN: pos.x--; break;
			case XP: pos.x++; break;
			case YN: pos.y--; break;
			case YP: pos.y++; break;
			case ZN: pos.z--; break;
			case ZP: pos.z++; break;
		}
		// void *neighbor = svo_get_node(pos, world->chunks);
		// if (is_node_valid(neighbor)) get_neighbor_face(&chk_build, i, (chunk_t *)node->data);
		//start_thread

	}
		
}

void	*haven_mesh_thread_task(void *arg) {
	chunk_builder_data_t *data = arg;
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int z = 0; z < CHUNK_SIZE; z++) {
			for (int y = 0; y < CHUNK_SIZE; y++) {
				//greedy mesh using mask with uint64_t
			}
		}
	}
}

void	haven_voxel_mesh_gen() {

}

void	haven_voxel_mesh_update() {

}

void	haven_voxel_mesh_clear() {

}
