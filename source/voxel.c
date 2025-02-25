#include <prototype.h>

face_data_t	pack_face_data(unsigned char pos[3], unsigned char face, unsigned char height, unsigned char width, unsigned short id) {
	face_data_t data;

	data.block_id = (id << 3 | face);
	data.face_data = ((height << 24) | (width << 18) | (pos[2] << 12) | (pos[1] << 6) | pos[0]);
	return (data);
}

//World Render

bool	is_chunk_valid(svo_node_t *node) {
	return node && node->data;
}

bool	is_solid_block(svo_node_t *node) {
	return node && node->data;
}

// Converts a world position (in world units) into voxel coordinates.
Vector3	world_to_voxel(const Vector3 worldPos) {
	return ((Vector3){floorf(worldPos.x * 2.0f), floorf(worldPos.y * 2.0f), floorf(worldPos.z * 2.0f)});
}

svo_node_t*	get_neighbor_block(Vector3 pos, FaceDirection dir, chunk_t *current, world_t *world) {
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

	
	// printf("%i\n", __LINE__);
	svo_node_t *node = svo_get_node(chunk_pos, world->chunks);
	if (!node)
		return (NULL);
	// printf("%i\n", __LINE__);
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

//add skiping mesh that aren't visible to player ??
void	gen_chunk_mesh(chunk_t *current, world_t *world) {
	for (char x = 0; x < CHUNK_SIZE; x++) {
		for (char z = 0; z < CHUNK_SIZE; z++) {
			for (char y = 0; y < CHUNK_SIZE; y++) {
				Vector3 pos = {x, y, z};
				
				svo_node_t *node = svo_get_node(pos, current->blocks);
				
				if (!is_chunk_valid(node)) continue;
				
				for (int i = 0; i < 6; i++) {
					svo_node_t *neighbor = get_neighbor_block(pos, i, current, world);
					
					if (!is_solid_block(neighbor)) {
						char packed_pos[3] = {x, y, z};
						face_data_t face = pack_face_data(packed_pos, i, 1, 1, 1);
						
						if (current->mesh->faces_count[i] < 4096) {
							current->mesh->faces[i][current->mesh->faces_count[i]++] = face;
						}
					}
				}
			}
		}
	}
}

void	update_chunk_mesh(Vector3 pos, world_t *world) {
	svo_node_t *node = svo_get_node(pos, world->chunks);
	if (node && node->data) {
		chunk_t *current_chunk = node->data;
		if (!current_chunk->mesh) {
			current_chunk->mesh = calloc(1, sizeof(chunk_mesh_t));
		} else {
			for (int i = 0; i < 6; i++) {
				memset(current_chunk->mesh->faces[i], 0, sizeof(face_data_t) * 4096);
				current_chunk->mesh->faces_count[i] = 0;
			}
		}
		gen_chunk_mesh(current_chunk, world);
	}
}

void	gen_world_mesh(world_t *world, engine_t *engine) {
	for (int x = 0; x < 8; x++) {
		for (int z = 0; z < 8; z++) {
			for (int y = 0; y < 8; y++) {
				svo_node_t *node = svo_get_node((Vector3){x,y,z}, world->chunks);
				if (is_solid_block(node)) {
					chunk_t *current_chunk = node->data;
					current_chunk->mesh = calloc(1, sizeof(chunk_mesh_t));
					gen_chunk_mesh(current_chunk, world);
				}
			}
		}
	}
}

bool	is_chunk_visible(Vector3 chunk_pos, Camera camera) {
	// Create view-projection matrix
	Matrix view = GetCameraViewMatrix(&camera);
	Matrix proj = GetCameraProjectionMatrix(&camera, GetScreenHeight()/ GetScreenWidth());
	Matrix view_proj = MatrixMultiply(view, proj);
	
	// Extract frustum
	Frustum frustum;
	ExtractFrustumFromMatrix(view_proj, &frustum);
	
	// Create chunk bounding box
	BoundingBox chunk_box = {
		.min = chunk_pos,
		.max = {chunk_pos.x + CHUNK_SIZE * VOXEL_SIZE, chunk_pos.y + CHUNK_SIZE * VOXEL_SIZE, chunk_pos.z + CHUNK_SIZE * VOXEL_SIZE}
	};
	
	return IsBoxInFrustum(chunk_box, frustum);
}

//check this every few frame;
void	update_world_render(world_t *world, engine_t *engine) {
	if (world->rcount) {
		for (int i = 0; i < 512; i++) {
			world->rqueue[i] = NULL;
		}
		world->rcount = 0;
	}
	for (int x = 0; x < 8; x++) {
		for (int z = 0; z < 8; z++) {
			for (int y = 0; y < 8; y++) {
				svo_node_t *node = svo_get_node((Vector3){x,y,z}, world->chunks);
				if (is_chunk_valid(node)) {
					chunk_t *current_chunk = node->data;
					if (is_chunk_visible(current_chunk->pos, engine->camera) && current_chunk->mesh->faces_count) {
						world->rqueue[world->rcount++] = current_chunk;
					}
				}
			}
		}
	}
}

void	setup_world_render(world_t *world, engine_t *engine) {
	if (world->rcount) {
		for (int i = 0; i < 512; i++) {
			world->rqueue[i] = NULL;
		}
		world->rcount = 0;
	}
	for (int x = 0; x < 8; x++) {
		for (int z = 0; z < 8; z++) {
			for (int y = 0; y < 8; y++) {
				svo_node_t *node = svo_get_node((Vector3){x,y,z}, world->chunks);
				if (is_chunk_valid(node)) {
					chunk_t *current_chunk = node->data;
					world->rqueue[world->rcount++] = current_chunk;
				}
			}
		}
	}
}

/*
need to refator after this to have correct nomenclature and also need to change some define and number
*/

// This function casts a ray from the camera’s center and returns the first solid voxel
// hit along the ray, up to max_range (which would be player->range).
bool	voxel_look_at(Camera3D camera, world_t *world, float max_range, Vector3 *position) {
	Ray ray = GetScreenToWorldRay((Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f }, camera);
	const float step = 0.1f;

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
		if (is_chunk_valid(chunk_node)) {
			chunk_t *chunk = chunk_node->data;
			if (chunk->blocks) {
				Vector3 local_pos = {(int)voxel.x % CHUNK_SIZE, (int)voxel.y % CHUNK_SIZE, (int)voxel.z % CHUNK_SIZE};
				if (is_solid_block(svo_get_node(local_pos, chunk->blocks))) {
					*position = Vector3Add((Vector3){0.25, 0.25, 0.25}, Vector3Multiply(voxel, (Vector3){0.5, 0.5, 0.5}));
					return (true);
				}
			}
		}
	}
	return (false);
}


// This function casts a ray from the camera’s center and change first hit solid voxel
// hit along the ray, up to max_range (which would be player->range).
void	voxel_set_block(Camera3D camera, world_t *world, float max_range, unsigned short id) {
	Ray ray = GetScreenToWorldRay((Vector2){ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f }, camera);
	const float step = 0.1f;

	for (float t = 0.0f; t < max_range; t += step) {
		Vector3 pos = Vector3Add(ray.position, Vector3Scale(ray.direction, t));
		Vector3 voxel = world_to_voxel(pos);

		Vector3 chunk_pos = {voxel.x / CHUNK_SIZE, voxel.y / CHUNK_SIZE, voxel.z / CHUNK_SIZE};
		svo_node_t* chunk_node = svo_get_node(chunk_pos, world->chunks);
		if (is_chunk_valid(chunk_node)) {
			chunk_t *chunk = chunk_node->data;
			if (chunk->blocks) {
				Vector3 local_pos = {(int)voxel.x % CHUNK_SIZE, (int)voxel.y % CHUNK_SIZE, (int)voxel.z % CHUNK_SIZE};
				// Vector3 local_pos = {voxel.x / CHUNK_SIZE, voxel.y / CHUNK_SIZE, voxel.z / CHUNK_SIZE};
				svo_node_t* vox_node = svo_get_node(local_pos, chunk->blocks);
				if (!id) {
					// printf ("delete at : %0.0f, %0.0f, %0.0f\n", local_pos.x, local_pos.y, local_pos.z);
					if (vox_node && vox_node->data) {
						free(vox_node->data);
						vox_node->data = NULL;
					}
					delete_node(vox_node);
				} else {
					if (vox_node && vox_node->data) {
						((voxel_t *)vox_node->data)->block_id = id;
					} else {
						voxel_t *vox = calloc(1, sizeof(voxel_t));
						vox->block_id = id;
						svo_insert(local_pos, vox, chunk->blocks);
					}
				}
				update_chunk_mesh(chunk_pos, world);
				update_chunk_render(chunk->mesh);
				return;
			}
		}
	}

}
