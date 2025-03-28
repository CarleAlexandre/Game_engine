#include "voxel_impl.h"

Vector3	world_to_voxel(const Vector3 worldPos) {
	return ((Vector3){floorf(worldPos.x * 2.0f), floorf(worldPos.y * 2.0f), floorf(worldPos.z * 2.0f)});
}

svo_node_t*	get_neighbor_block(Vector3 pos, char dir, chunk_t *current, world_t *world) {
	Vector3 neighbor_pos = pos;
	
	switch(dir) {
	    case FACE_YP: neighbor_pos.y++; break;
	    case FACE_Y:  neighbor_pos.y--; break;
	    case FACE_XP: neighbor_pos.x++; break;
	    case FACE_X:  neighbor_pos.x--; break;
	    case FACE_ZP: neighbor_pos.z++; break;
	    case FACE_Z:  neighbor_pos.z--; break;
	}

	if (!current || !current->blocks)
		return NULL;

	if (neighbor_pos.x >= 0 && neighbor_pos.x < CHUNK_SIZE &&
		neighbor_pos.y >= 0 && neighbor_pos.y < CHUNK_SIZE &&
		neighbor_pos.z >= 0 && neighbor_pos.z < CHUNK_SIZE) {
		return svo_get_node(neighbor_pos, current->blocks);
	}
    
	Vector3 chunk_pos = {
		(current->pos.x / 32) + ((dir == FACE_XP) ? 1 : (dir == FACE_X) ? -1 : 0),
		(current->pos.y / 32) + ((dir == FACE_YP) ? 1 : (dir == FACE_Y) ? -1 : 0),
		(current->pos.z / 32) + ((dir == FACE_ZP) ? 1 : (dir == FACE_Z) ? -1 : 0)
	};

	svo_node_t *node = svo_get_node(chunk_pos, world->chunks);
	if (!node)
		return (NULL);
	chunk_t *neighbor_chunk = node->data;
	if (!neighbor_chunk || !neighbor_chunk->blocks)
		return NULL;

	Vector3 local_pos = {
		(dir == FACE_XP) ? 0 : (dir == FACE_X) ? CHUNK_SIZE - 1 : pos.x,
		(dir == FACE_YP) ? 0 : (dir == FACE_Y) ? CHUNK_SIZE - 1 : pos.y,
		(dir == FACE_ZP) ? 0 : (dir == FACE_Z) ? CHUNK_SIZE - 1 : pos.z
	};
	
	return svo_get_node(local_pos, neighbor_chunk->blocks);
}

void	get_neighbor_face(chunk_builder_data_t *chk_build, char dir, chunk_t *neighbor) {
	Vector3 pos;
	for (int k = 0; k < 64; k++) {
		for (char i = 0; i < 64; i++) {
			switch(dir) {
				case FACE_YP: pos.x = k; pos.y = 0; pos.z = i; break;
				case FACE_Y: pos.x = k; pos.y = 63; pos.z = i; break;
				case FACE_XP: pos.x = 0; pos.y = i; pos.z = k; break;
				case FACE_X:  pos.x = 63; pos.y = i; pos.z = k; break;
				case FACE_ZP: pos.x = k; pos.y = i; pos.z = 0; break;
				case FACE_Z:  pos.x = k; pos.y = i; pos.z = 63; break;
			}
			svo_node_t *node = svo_get_node(pos, neighbor->blocks);
			chk_build->face[i] += (is_node_valid(node) ? 1 : 0) << i;
		}
	}
}

void	get_block_data(chunk_builder_data_t *chk_build, svo_t *blocks) {
	for (int x = 0; x < 64; x++) {
		for (char z = 0; z < 64; z++) {
			for (char y = 0; y < 64; y++) {
				svo_node_t *node = svo_get_node((Vector3){x, y, z}, blocks);
				chk_build->block_data[x][z] += (is_node_valid(node) ? 1 : 0) << y;
			}
		}
	}
}
