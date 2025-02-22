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

void get_chunk_neighbor(Vector3 pos, svo_t *tree, chunk_t *chunk[6]) {
	float nx, ny, nz;

	nx = pos.x;
	ny = pos.y;
	nz = pos.z;

	chunk[0] = (chunk_t *)svo_get_node((Vector3){nx, ny + 1, nz}, tree);
	chunk[1] = (chunk_t *)svo_get_node((Vector3){nx, ny - 1, nz}, tree);
	chunk[2] = (chunk_t *)svo_get_node((Vector3){nx + 1, ny, nz}, tree);
	chunk[3] = (chunk_t *)svo_get_node((Vector3){nx - 1, ny, nz}, tree);
	chunk[4] = (chunk_t *)svo_get_node((Vector3){nx, ny, nz + 1}, tree);
	chunk[5] = (chunk_t *)svo_get_node((Vector3){nx, ny, nz - 1}, tree);
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
		current->pos.x + ((dir == FACE_XP) ? 1 : (dir == FACE_X) ? -1 : 0),
		current->pos.y + ((dir == FACE_YP) ? 1 : (dir == FACE_Y) ? -1 : 0),
		current->pos.z + ((dir == FACE_ZP) ? 1 : (dir == FACE_Z) ? -1 : 0)
	};

	if (chunk_pos.x < 0 || chunk_pos.x >= 8 ||
		chunk_pos.y < 0 || chunk_pos.y >= 8 || 
		chunk_pos.z < 0 || chunk_pos.z >= 8) {
		return NULL;
	}
	svo_node_t *node = svo_get_node(chunk_pos, world->tree);
	if (!node) return (NULL);
	chunk_t *neighbor_chunk = node->data;
	if (!neighbor_chunk || !neighbor_chunk->blocks) return NULL;

	Vector3 local_pos = {
		(dir == FACE_XP) ? 0 : (dir == FACE_X) ? 63 : pos.x,
		(dir == FACE_YP) ? 0 : (dir == FACE_Y) ? 63 : pos.y,
		(dir == FACE_ZP) ? 0 : (dir == FACE_Z) ? 63 : pos.z
	};
	
	return svo_get_node(local_pos, neighbor_chunk->blocks);
}


void gen_chunk_mesh(chunk_t *current, world_t *world) {
	const FaceDirection directions[6] = {FACE_YP, FACE_Y, FACE_XP, FACE_X, FACE_ZP, FACE_Z};
	
	for (char x = 0; x < 64; x++) {
		for (char z = 0; z < 64; z++) {
			for (char y = 0; y < 64; y++) {
				Vector3 pos = {x, y, z};
				svo_node_t *node = svo_get_node(pos, current->blocks);
				
				if (!is_solid_block(node)) continue;

				for (int i = 0; i < 6; i++) {
					FaceDirection dir = directions[i];
					svo_node_t *neighbor = get_neighbor_block(pos, dir, current, world);
					
					if (!is_solid_block(neighbor)) {
						char packed_pos[3] = {x, y, z};
						face_data_t face = pack_face_data(packed_pos, dir, 1, 1, 1);
						
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
					printf("%i\n", current_chunk->mesh->faces_count);
				}
			}
		}
	}
}


// bool is_chunk_visible(Vector3 chunk_pos, Camera camera) {
// 	// Convert chunk position to world coordinates
// 	BoundingBox chunk_bb = (BoundingBox){
// 	    .min = Vector3Add(chunk_pos, (Vector3){0, 0, 0}),
// 	    .max = Vector3Add(chunk_pos, (Vector3){64, 64, 64})
// 	};
	
// 	return CheckCollisionBoxFrustum(chunk_bb, GetCameraFrustum(&camera));
//     }
    
//     void update_world_render(world_t *world, engine_t *engine) {
// 	world->rcount = 0;
	
// 	for (int x = 0; x < 4; x++) {
// 	    for (int z = 0; z < 4; z++) {
// 		for (int y = 0; y < 4; y++) {
// 		    svo_node_t *node = svo_get_node((Vector3){x, y, z}, world->tree);
// 		    if (node && node->data) {
// 			chunk_t *chunk = node->data;
			
// 			if (is_chunk_visible(chunk->pos, engine->camera)) {
// 			    if (world->rcount < 100000) {
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
	Matrix proj = GetCameraProjectionMatrix(&camera, GetScreenWidth()/ GetScreenHeight());
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
		for (int i = 0; i < 128; i++) {
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
					// if (is_chunk_visible(current_chunk->pos, engine->camera) && current_chunk->mesh->faces_count) {
						world->rqueue[world->rcount++] = current_chunk;
					// }
				}
			}
		}
	}
}
