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

void	*mesh_thrd(void *arg) {
	chunk_builder_data_t *data = arg;
	for (int x = 0; x < CHUNK_SIZE; x++) {
		for (int z = 0; z < CHUNK_SIZE; z++) {
			for (int y = 0; y < CHUNK_SIZE; y++) {
				//greedy mesh using mask with uint64_t
			}
		}
	}
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

void	gen_chunk_greedymesh(chunk_t *current, world_t *world) {
	chunk_builder_data_t chk_build = {0};//need to be mutexed or duplicated
	
	Vector3 pos = {current->pos.x / 32, current->pos.y / 32, current->pos.z / 32};
	svo_node_t *node = svo_get_node(pos, world->chunks);
	if (!is_node_valid(node)) return;
	get_block_data(&chk_build, ((chunk_t *)node->data)->blocks);
	for (int i = 0; i < 6; i++) {
		current->mesh->faces_count[i] = 0;
		pos = (Vector3){current->pos.x / 32, current->pos.y / 32, current->pos.z / 32};
		switch(i) {
			case FACE_YP: pos.y++; break;
			case FACE_Y:  pos.y--; break;
			case FACE_XP: pos.x++; break;
			case FACE_X:  pos.x--; break;
			case FACE_ZP: pos.z++; break;
			case FACE_Z:  pos.z--; break;
		}
		svo_node_t *neighbor = svo_get_node(pos, world->chunks);
		if (is_node_valid(neighbor)) get_neighbor_face(&chk_build, i, (chunk_t *)node->data);
		//start_thread

	}
		
}

/*
still not very good, but for debug issue good enough
*/
bool	voxel_look_at(Camera3D camera, world_t *world, float max_range, Vector3 *position) {
	Ray ray = GetScreenToWorldRay((Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f }, camera);
	const float step = 0.01f;

	for (float t = 0.0f; t < max_range; t += step) {
		Vector3 pos = Vector3Add(ray.position, Vector3Scale(ray.direction, t));
		Vector3 voxel = world_to_voxel(pos);

		Vector3 chunk_pos = {voxel.x / CHUNK_SIZE, voxel.y / CHUNK_SIZE, voxel.z / CHUNK_SIZE};
		if (voxel.x < 0 && ((int)voxel.x % 64) != 0)
			chunk_pos.x--;
		if (voxel.y < 0 && ((int)voxel.y % 64) != 0)
			chunk_pos.y--;
		if (voxel.z < 0 && ((int)voxel.z % 64) != 0)
			chunk_pos.z--;
		svo_node_t* chunk_node = svo_get_node(chunk_pos, world->chunks);
		if (is_node_valid(chunk_node)) {
			chunk_t *chunk = chunk_node->data;
			if (chunk->blocks) {
				Vector3 local_pos = {(int)voxel.x % CHUNK_SIZE, (int)voxel.y % CHUNK_SIZE, (int)voxel.z % CHUNK_SIZE};
				if (is_node_valid(svo_get_node(local_pos, chunk->blocks))) {
					*position = Vector3Add((Vector3){0.25, 0.25, 0.25}, Vector3Multiply(voxel, (Vector3){0.5, 0.5, 0.5}));
					return (true);
				}
			}
		}
	}
	return (false);
}

void	voxel_destroy_block(Camera3D camera, world_t *world, float max_range) {
	Ray ray = GetScreenToWorldRay((Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f }, camera);
	const float step = 0.01f;
    
	for (float t = 0.0f; t < max_range; t += step) {
		Vector3 pos = Vector3Add(ray.position, Vector3Scale(ray.direction, t));
		Vector3 voxel = world_to_voxel(pos);
		Vector3 chunk_pos = {
			floorf(voxel.x / CHUNK_SIZE),
			floorf(voxel.y / CHUNK_SIZE),
			floorf(voxel.z / CHUNK_SIZE)
		};
		svo_node_t *chunk_node = svo_get_node(chunk_pos, world->chunks);
		if (is_node_valid(chunk_node)) {
			chunk_t *chunk = chunk_node->data;
			if (chunk->blocks) {
				Vector3 local_pos = {
					(int)fmodf(voxel.x, CHUNK_SIZE),
					(int)fmodf(voxel.y, CHUNK_SIZE),
					(int)fmodf(voxel.z, CHUNK_SIZE)
				};
				svo_node_t *vox_node = svo_get_node(local_pos, chunk->blocks);
				if (is_node_valid(vox_node)) {
					free(vox_node->data);
					vox_node->data = NULL;
					delete_node(vox_node);
					update_chunk_mesh(chunk_pos, world);
					update_chunk_render(chunk->mesh);
					return;
				}
			}
		}
	}
}

void	voxel_place_block(Camera3D camera, world_t *world, float max_range, unsigned short id) {
	Ray ray = GetScreenToWorldRay((Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f }, camera);
	const float step = 0.01f;
	Vector3 last_empty_voxel = {0, 0, 0};
	bool last_empty_found = false;

	for (float t = 0.0f; t < max_range; t += step) {
		Vector3 pos = Vector3Add(ray.position, Vector3Scale(ray.direction, t));
		Vector3 voxel = world_to_voxel(pos);
		Vector3 chunk_pos = {
			floorf(voxel.x / CHUNK_SIZE),
			floorf(voxel.y / CHUNK_SIZE),
			floorf(voxel.z / CHUNK_SIZE)
		};
		svo_node_t *chunk_node = svo_get_node(chunk_pos, world->chunks);
		if (is_node_valid(chunk_node)) {
			chunk_t *chunk = chunk_node->data;
			if (chunk->blocks) {
				Vector3 local_pos = {
					(int)fmodf(voxel.x, CHUNK_SIZE),
					(int)fmodf(voxel.y, CHUNK_SIZE),
					(int)fmodf(voxel.z, CHUNK_SIZE)
				};
				svo_node_t *vox_node = svo_get_node(local_pos, chunk->blocks);
				if (vox_node && vox_node->data) {
					if (last_empty_found) {
						Vector3 place_voxel = last_empty_voxel;
						Vector3 place_chunk_pos = {
							floorf(place_voxel.x / CHUNK_SIZE),
							floorf(place_voxel.y / CHUNK_SIZE),
							floorf(place_voxel.z / CHUNK_SIZE)
						};
						svo_node_t *place_chunk_node = svo_get_node(place_chunk_pos, world->chunks);
						if (is_node_valid(place_chunk_node)) {
							chunk_t *place_chunk = place_chunk_node->data;
							if (place_chunk->blocks) {
								Vector3 local_place = {
									(int)fmodf(place_voxel.x, CHUNK_SIZE),
									(int)fmodf(place_voxel.y, CHUNK_SIZE),
									(int)fmodf(place_voxel.z, CHUNK_SIZE)
								};
								svo_node_t *check = svo_get_node(local_place, place_chunk->blocks);
								if (!(check && check->data)) {
									voxel_t *new_voxel = calloc(1, sizeof(voxel_t));
									new_voxel->block_id = id;
									svo_insert(local_place, new_voxel, place_chunk->blocks);
									update_chunk_mesh(place_chunk_pos, world);
									update_chunk_render(place_chunk->mesh);
								}
							}
						}
					}
					return;
				} else {
					last_empty_voxel = voxel;
					last_empty_found = true;
				}
			}
		}
	}
}
