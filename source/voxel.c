#include <prototype.h>

face_data_t	pack_face_data(unsigned char pos[3], unsigned char face, unsigned char height, unsigned char width, unsigned short id) {
	face_data_t data;

	data.block_id = (id << 3 | face);
	data.face_data = ((height << 24) | (width << 18) | (pos[2] << 12) | (pos[1] << 6) | pos[0]);
	return (data);
}

//World Render

bool is_solid_block(svo_node_t *node) {
	return node && node->data;
}

svo_node_t* get_neighbor_block(Vector3 pos, FaceDirection dir, chunk_t *current, world_t *world) {
	Vector3 neighbor_pos = pos;
	
	switch(dir) {
	    case FACE_YP: neighbor_pos.y++; break;
	    case FACE_Y:  neighbor_pos.y--; break;
	    case FACE_XP: neighbor_pos.x++; break;
	    case FACE_X:  neighbor_pos.x--; break;
	    case FACE_ZP: neighbor_pos.z++; break;
	    case FACE_Z:  neighbor_pos.z--; break;
	}

	if (!current || !current->blocks) return NULL;

	if (neighbor_pos.x >= 0 && neighbor_pos.x < 64 &&
		neighbor_pos.y >= 0 && neighbor_pos.y < 64 &&
		neighbor_pos.z >= 0 && neighbor_pos.z < 64) {
		return svo_get_node(neighbor_pos, current->blocks);
	}
    
	Vector3 chunk_pos = {
		(current->pos.x / 32) + ((dir == FACE_XP) ? 1 : (dir == FACE_X) ? -1 : 0),
		(current->pos.y / 32) + ((dir == FACE_YP) ? 1 : (dir == FACE_Y) ? -1 : 0),
		(current->pos.z / 32) + ((dir == FACE_ZP) ? 1 : (dir == FACE_Z) ? -1 : 0)
	};

	svo_node_t *node = svo_get_node(chunk_pos, world->tree);
	if (!node)
		return (NULL);
	chunk_t *neighbor_chunk = node->data;
	if (!neighbor_chunk || !neighbor_chunk->blocks)
		return NULL;

	Vector3 local_pos = {
		(dir == FACE_XP) ? 0 : (dir == FACE_X) ? 63 : pos.x,
		(dir == FACE_YP) ? 0 : (dir == FACE_Y) ? 63 : pos.y,
		(dir == FACE_ZP) ? 0 : (dir == FACE_Z) ? 63 : pos.z
	};
	
	return svo_get_node(local_pos, neighbor_chunk->blocks);
}

//add skiping mesh that aren't visible to player ??
void gen_chunk_mesh(chunk_t *current, world_t *world) {
	for (char x = 0; x < 64; x++) {
		for (char z = 0; z < 64; z++) {
			for (char y = 0; y < 64; y++) {
				Vector3 pos = {x, y, z};
				svo_node_t *node = svo_get_node(pos, current->blocks);
				
				if (!is_solid_block(node)) continue;

				for (int i = 0; i < 6; i++) {
					svo_node_t *neighbor = get_neighbor_block(pos, i, current, world);
					
					if (!is_solid_block(neighbor)) {
						char packed_pos[3] = {x, y, z};
						face_data_t face = pack_face_data(packed_pos, i, 1, 1, 1);
						
						if (current->mesh->faces_count < 100000) {
							current->mesh->faces[current->mesh->faces_count++] = face;
						}
					}
				}
			}
		}
	}
}

void	gen_world_mesh(world_t *world, engine_t *engine) {
	for (int x = 0; x < 8; x++) {
		for (int z = 0; z < 8; z++) {
			for (int y = 0; y < 8; y++) {
				svo_node_t *node = svo_get_node((Vector3){x,y,z}, world->tree);
				if (node && node->data) {
					chunk_t *current_chunk = node->data;
					current_chunk->mesh = malloc(sizeof(chunk_mesh_t));
					gen_chunk_mesh(current_chunk, world);
				}
			}
		}
	}
}
    
// void update_world_render(world_t *world, engine_t *engine) {
// 	world->rcount = 0;
	
// 	for (int x = 0; x < world->tree->size; x++) {
// 	    for (int z = 0; z < world->tree->size; z++) {
// 		for (int y = 0; y < world->tree->size; y++) {
// 		    svo_node_t *node = svo_get_node((Vector3){x, y, z}, world->tree);
// 		    if (node && node->data) {
// 			chunk_t *chunk = node->data;
			
// 			if (is_chunk_visible(chunk->pos, engine->camera)) {
// 			    if (world->rcount < 512) {
// 				world->rqueue[world->rcount++] = chunk;
    
// 				if (!chunk->mesh) {
// 				    chunk->mesh = malloc(sizeof(chunk_mesh_t));
// 				    gen_chunk_mesh(chunk, world);
// 				}
// 			    }
// 			}
// 		    }
// 		}
// 	    }
// 	}
// }

bool is_chunk_visible(Vector3 chunk_pos, Camera camera) {
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
		.max = {chunk_pos.x + 32, chunk_pos.y + 32, chunk_pos.z + 32}
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
				svo_node_t *node = svo_get_node((Vector3){x,y,z}, world->tree);
				if (node && node->data) {
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
				svo_node_t *node = svo_get_node((Vector3){x,y,z}, world->tree);
				if (node && node->data) {
					chunk_t *current_chunk = node->data;
					world->rqueue[world->rcount++] = current_chunk;
				}
			}
		}
	}
}
