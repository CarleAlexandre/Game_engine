#include <prototype.h>

face_data_t	pack_face_data(unsigned char pos[3], unsigned char face, unsigned char height, unsigned char width, unsigned short id) {
	face_data_t data;

	data.block_id = (id << 3 | face);
	data.face_data = ((height << 24) | (width << 18) | (pos[2] << 12) | (pos[1] << 6) | pos[0]);
	return (data);
}

int	get_vox_idx(int x, int y, int z) {
	return (x + (z * 64) + (y * 64 * 64));
}

Vector3	get_vox_pos(int idx) {
	return ((Vector3){idx % 64, (idx /64) % 64, ((idx / 64)/ 64) % 64});
}

// each world part are 2048 vox^3 or 64 chunk ^ 3 but it's an octree

// only part that are heavily player edited will be save and kept in memory else it will regenerate that will make use save lot of disk space

//'also dungeon should not be kept but genereted when entered and cleared when outside (else i had mecanics for player to go back inside)

//gen chunk mesh

static inline int get_x(int face, int i, int j, int depth) {
	switch(face) {
	    case FACE_LEFT:  return depth;
	    case FACE_RIGHT: return depth;
	    case FACE_FRONT: return i;
	    case FACE_BACK:  return i;
	    default:        return i;
	}
}

static inline int get_y(int face, int i, int j, int depth) {
	switch(face) {
		case FACE_TOP:    return depth;
		case FACE_BOTTOM: return depth;
		case FACE_FRONT: return j;
		case FACE_BACK:  return j;
		default:        return j;
	}
}

static inline int get_z(int face, int i, int j, int depth) {
	switch(face) {
		case FACE_LEFT:  return j;
		case FACE_RIGHT: return j;
		case FACE_FRONT: return depth;
		case FACE_BACK:  return depth;
		default:        return depth;
	}
}

int	get_neighbor_id(chunk_t *chunk, int x, int y, int z, int face) {
	int nx = x, ny = y, nz = z;
	switch(face) {
	    case FACE_TOP: ny++; break;
	    case FACE_BOTTOM: ny--; break;
	    case FACE_LEFT: nx--; break;
	    case FACE_RIGHT: nx++; break;
	    case FACE_FRONT: nz++; break;
	    case FACE_BACK: nz--; break;
	}

	if(nx < 0 || nx >= 32 || nz < 0 || nz >= 32 || ny < 0 || ny >= 32)
	    return(0);
	
	return(chunk->blocks[get_vox_idx(nx, nz, ny)]->block_id);
}

face_data_t	add_face_to_mesh(int x, int y, int z, int face, int id, char width, char height) {
	char pos[3];

	switch(face) {
		case FACE_TOP:
			pos[0] = x;
			pos[1] = y + 1;
			pos[2] = z;
			break;
		case FACE_BOTTOM:
			pos[0] = x;
			pos[1] = y;
			pos[2] = z;
			break;
		case FACE_LEFT:
			pos[0] = x;
			pos[1] = y;
			pos[2] = z;
			break;
		case FACE_RIGHT:
			pos[0] = x + width;
			pos[1] = y;
			pos[2] = z;
			break;
		case FACE_FRONT:
			pos[0] = x;
			pos[1] = y;
			pos[2] = z + height;
			break;
		case FACE_BACK:
			pos[0] = x;
			pos[1] = y;
			pos[2] = z;
			break;
	}

	return (pack_face_data(pos, face, height, width, id));
}

//does it even work ???
void	greedy_mesh_face(chunk_t *chunk, vox_mesh_t *mesh, int face) {
	bool mask[64][64] = {0}; // Visibility mask for current layer
	face_data_t faces[100000] = {0};
	int current_idx = 0;
	int primary, secondary, depth;
	// Axis mapping based on face direction
	for(depth = 0; depth < 64; depth++) {
		// Generate visibility mask for current layer
		for(primary = 0; primary < 64; primary++) {
			for(secondary = 0; secondary < 64; secondary++) {
				int x, y, z;
				switch(face) {
					case FACE_TOP:    x = primary; z = secondary; y = depth; break;
					case FACE_BOTTOM: x = primary; z = secondary; y = depth; break;
					case FACE_LEFT:  y = primary; z = secondary; x = depth; break;
					case FACE_RIGHT: y = primary; z = secondary; x = depth; break;
					case FACE_FRONT: x = primary; y = secondary; z = depth; break;
					case FACE_BACK:  x = primary; y = secondary; z = depth; break;
				}

				int current_id = chunk->blocks[get_vox_idx(x,y,z)]->block_id;
				if(current_id == 0) {
					mask[primary][secondary] = false;
					continue;
				}

				int neighbor_id = get_neighbor_id(chunk, x, y, z, face);
				mask[primary][secondary] = (current_id != neighbor_id);
			}
		}

		// Process the mask to find rectangular regions
		for(int j = 0; j < 64; j++) {
			for(int i = 0; i < 64; i++) {
				if(!mask[i][j]) continue;

				// Find maximum width
				int w;
				for(w = 1; i + w < 64 && mask[i + w][j] && 
				chunk->blocks[get_vox_idx(get_x(face,i + w,j,depth), get_y(face,i + w,j,depth), get_z(face,i + w,j,depth))]->block_id == 
				chunk->blocks[get_vox_idx(get_x(face,i,j,depth), get_y(face,i,j,depth), get_z(face,i,j,depth))]->block_id; w++);

				// Find maximum height
				int h;
				for(h = 1; j + h < 64; h++) {
					for(int k = 0; k < w; k++) {
						if(!mask[i + k][j + h] || 
						chunk->blocks[get_vox_idx(get_x(face,i + k,j + h,depth), get_y(face,i + k,j + h,depth), get_z(face,i + k,j + h,depth))]->block_id != 
						chunk->blocks[get_vox_idx(get_x(face,i,j,depth), get_y(face,i,j,depth), get_z(face,i,j,depth))]->block_id) {
							goto done_height;
						}
					}
				}
				done_height:

				// Add merged face
				int x = get_x(face,i,j,depth);
				int y = get_y(face,i,j,depth);
				int z = get_z(face,i,j,depth);
				int id = chunk->blocks[get_vox_idx(x, y, z)]->block_id;
				
				faces[current_idx++] = add_face_to_mesh(x, y, z, face, id, w, h);

				// Mark region as processed
				for(int a = 0; a < w; a++) {
					for(int b = 0; b < h; b++) {
						mask[i + a][j + b] = false;
					}
				}
			}
		}
	}

	face_data_t *new_faces = malloc(sizeof(face_data_t) * (mesh->face_count + current_idx));
	memset(new_faces, 0, sizeof(face_data_t) * (mesh->face_count + current_idx));
	if (mesh->faces) {
		memcpy(new_faces, mesh->faces, sizeof(face_data_t) * mesh->face_count);
		free (mesh->faces);
	}
	mesh->faces = new_faces;
	for (int i = 0; i < current_idx; i++) {
		mesh->faces[mesh->face_count + i] = faces[i];
	}
}


static inline int	num_cmp(int f1, int f2) {
	if (f1 > f2) {
		return (1);
	} else if (f1 < f2) {
		return (2);
	}
	return (0);
}

chunk_render_t	*generate_chunk_mesh(chunk_t *chunk, vox_mesh_t *mesh, Vector3 pos) {
	chunk_render_t	*render;

	char	face[3][2] = {{FACE_LEFT, FACE_TOP}, {FACE_BOTTOM, FACE_TOP}, {FACE_BACK, FACE_FRONT}};
	int	c_pos[3] = {chunk->x, chunk->y, chunk->z}, p_pos[3];
	p_pos[0] = pos.x;
	p_pos[1] = pos.y;
	p_pos[2] = pos.z;

	render->face_offset = mesh->face_count;

	for (int i = 0; i < 3; i++) {
		switch(num_cmp(c_pos[i], p_pos[i])) {
			case (0):
				greedy_mesh_face(chunk, mesh, face[i][0]);
				render->face_count++;
				break;
			case (1):
				greedy_mesh_face(chunk, mesh, face[i][1]);
				render->face_count++;
				break;
			case (2):
				greedy_mesh_face(chunk, mesh, face[i][0]);
				greedy_mesh_face(chunk, mesh, face[i][1]);
				render->face_count += 2;
				break;
		}
	}
	render->x = chunk->x;
	render->y = chunk->y;
	render->z = chunk->z;
	return (render);
}

//World Render

bool	is_chunk_visible(BoundingBox *bbox, const plane_t frustum[6]) {
	for (int i = 0; i < 6; i++) {
		Vector3	positive_corner = bbox->min;
		if (frustum[i].normal.x >= 0) {
			positive_corner.x = bbox->max.x;
		}
		if (frustum[i].normal.y >= 0) {
			positive_corner.y = bbox->max.y;
		}
		if (frustum[i].normal.z >= 0) {
			positive_corner.z = bbox->max.z;
		}
    
		float distance = Vector3DotProduct(frustum[i].normal, positive_corner) + frustum[i].distance;
		if (distance < 0) {
		    return(false);
		}
	}
	return(true);
}

chunk_t* get_chunk_at_position(chunk_t **chunks, Vector3 position) {
	int chunk_x = (int)(position.x / 16);
	int chunk_y = (int)(position.y / 16);
	int chunk_z = (int)(position.z / 16);
    
	if (chunk_x < 0 || chunk_x >= 128 || chunk_y < 0 || chunk_y >= 128 || chunk_z < 0 || chunk_z >= 64) {
	    return(NULL);
	}
    
	return (chunks[chunk_x + chunk_y + chunk_z]);// need to get chunk pos
}
    
voxel_t* get_voxel_at_position(chunk_t *chunk, Vector3 position) {
	if (!chunk)
		return(NULL);
    
	int voxel_x = (int)(position.x * 2.0f) % 32;
	int voxel_y = (int)(position.y * 2.0f) % 32;
	int voxel_z = (int)(position.z * 2.0f) % 32;
    
	return chunk->blocks[get_vox_idx(voxel_x, voxel_y, voxel_z)];
}

//using dda
bool raycast(Ray ray, chunk_t **chunks) {
	Vector3 ray_pos = ray.position;
	Vector3 ray_dir = Vector3Normalize(ray.direction);
    
	Vector3 step = {
		(ray_dir.x > 0) ? 0.5f : -0.5f,
		(ray_dir.y > 0) ? 0.5f : -0.5f,
		(ray_dir.z > 0) ? 0.5f : -0.5f
	};

	Vector3 delta_dist = {
		fabsf(0.5f / ray_dir.x),
		fabsf(0.5f / ray_dir.y),
		fabsf(9.5f / ray_dir.z)
	};

	Vector3 side_dist;
	side_dist.x = (ray_dir.x > 0) ? (ceilf(ray_pos.x) - ray_pos.x) : (ray_pos.x - floorf(ray_pos.x));
	side_dist.y = (ray_dir.y > 0) ? (ceilf(ray_pos.y) - ray_pos.y) : (ray_pos.y - floorf(ray_pos.y));
	side_dist.z = (ray_dir.z > 0) ? (ceilf(ray_pos.z) - ray_pos.z) : (ray_pos.z - floorf(ray_pos.z));
	side_dist = Vector3Multiply(side_dist, delta_dist);

	while (true) {
		chunk_t *current_chunk = get_chunk_at_position(chunks, ray_pos);
		if (current_chunk) {
			voxel_t *current_voxel = get_voxel_at_position(current_chunk, ray_pos);
			if (current_voxel && current_voxel->block_id != 0) {
				return(true);
			}
		}

		if (side_dist.x < side_dist.y && side_dist.x < side_dist.z) {
			ray_pos.x += step.x;
			side_dist.x += delta_dist.x;
		} else if (side_dist.y < side_dist.z) {
			ray_pos.y += step.y;
			side_dist.y += delta_dist.y;
		} else {
			ray_pos.z += step.z;
			side_dist.z += delta_dist.z;
		}

		float max_distance = 1024.0f;//need to chgeck if it's good or not render distance
		if (Vector3Distance(ray.position, ray_pos) > max_distance) {
			return(false);
		}
	}
	return(false);
}

bool	is_chunk_occluded(chunk_t *chunk, Vector3 camera_position, chunk_t **chunks) {
	Vector3	chunk_center;
	Ray	ray;

	chunk_center = Vector3Add(chunk->bounding_box.min, chunk->bounding_box.max);
	chunk_center = Vector3Divide(chunk_center, (Vector3){0.5, 0.5, 0.5});	
	ray.direction = Vector3Normalize(Vector3Subtract(chunk_center, camera_position));
	ray.position = camera_position;
	
	return (raycast(ray, chunks));
}

void	gen_render_chunk(world_t *world, engine_t *engine) {
	float cx = engine->camera.position.x / 16;
	float cy = engine->camera.position.y / 16;
	float cz = engine->camera.position.z / 16;

	Vector3 forward = GetCameraForward(&engine->camera);

	Matrix view_proj = GetCameraProjectionMatrix(&engine->camera, GetRenderHeight()/ GetRenderWidth());

	plane_t frustum[6];
	extract_frustum_planes(view_proj, &frustum[0]);

	for (int idx = 0; idx < world; idx++) {
		chunk_t *current_chunk = world->chunk[idx];
		if (is_chunk_visible(&current_chunk->bounding_box, frustum) && !is_chunk_occluded(current_chunk, engine->camera.position, world->chunk)) {
			chunk_render_t *rend = generate_chunk_mesh(current_chunk, &engine->render.world.mesh, engine->camera.position);
			dyn_add_elem(engine->render.world.rqueue, rend);
		}
	}
}

void	reload_voxel_world(world_t *world, engine_t *engine) {
	gen_render_chunk(world, engine);
	reload_world_vao(&engine->render.world);
	reload_world_ssbo(&engine->render.world);
	reload_indirect_buffer(&engine->render);
}

void	set_block(chunk_t *chunk, int x, int y, int z, voxel_t *vox) {
	
}

void	update_block(chunk_t *chunk, int x, int y, int z, voxel_t *vox) {
	
}
