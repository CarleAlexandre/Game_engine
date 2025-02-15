#define FNL_IMPL
#include "engine.h"

face_data_t	pack_face_data(char pos[3], char face, char height, char width, int id) {
	face_data_t data;

	data.block_id = id;
	data.face_data = ((height << 23) | (width << 18) | (face << 15) | (pos[2] << 10) | (pos[1] << 5) | (pos[0]));
	return (data);
}

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
	
	return(chunk->blocks[nx][nz][ny]->block_id);
}

void	add_face_to_mesh(mesh_t *mesh, int x, int y, int z, int face, int id, char width, char height) {
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

	face_data_t face_data = pack_face_data(pos, face, height, width, id);
	if (mesh->face_count < 99999) {
		mesh->faces[mesh->face_count++] = face_data;
	}
}

void	greedy_mesh_face(chunk_t *chunk, mesh_t *mesh, int face) {
	bool mask[32][32] = {0}; // Visibility mask for current layer
	int primary, secondary, depth;

	// Axis mapping based on face direction
	for(depth = 0; depth < 32; depth++) {
		// Generate visibility mask for current layer
		for(primary = 0; primary < 32; primary++) {
			for(secondary = 0; secondary < 32; secondary++) {
				int x, y, z;
				switch(face) {
					case FACE_TOP:    x = primary; z = secondary; y = depth; break;
					case FACE_BOTTOM: x = primary; z = secondary; y = depth; break;
					case FACE_LEFT:  y = primary; z = secondary; x = depth; break;
					case FACE_RIGHT: y = primary; z = secondary; x = depth; break;
					case FACE_FRONT: x = primary; y = secondary; z = depth; break;
					case FACE_BACK:  x = primary; y = secondary; z = depth; break;
				}

				int current_id = chunk->blocks[x][z][y]->block_id;
				if(current_id == 0) {
					mask[primary][secondary] = false;
					continue;
				}

				int neighbor_id = get_neighbor_id(chunk, x, y, z, face);
				mask[primary][secondary] = (current_id != neighbor_id);
			}
		}

		// Process the mask to find rectangular regions
		for(int j = 0; j < 32; j++) {
			for(int i = 0; i < 32; i++) {
				if(!mask[i][j]) continue;

				// Find maximum width
				int w;
				for(w = 1; i + w < 32 && mask[i + w][j] && 
				chunk->blocks[get_x(face,i + w,j,depth)][get_z(face,i + w,j,depth)][get_y(face,i + w,j,depth)]->block_id == 
				chunk->blocks[get_x(face,i,j,depth)][get_z(face,i,j,depth)][get_y(face,i,j,depth)]->block_id; w++);

				// Find maximum height
				int h;
				for(h = 1; j + h < 32; h++) {
					for(int k = 0; k < w; k++) {
						if(!mask[i + k][j + h] || 
						chunk->blocks[get_x(face,i + k,j + h,depth)][get_z(face,i + k,j + h,depth)][get_y(face,i + k,j + h,depth)]->block_id != 
						chunk->blocks[get_x(face,i,j,depth)][get_z(face,i,j,depth)][get_y(face,i,j,depth)]->block_id) {
							goto done_height;
						}
					}
				}
				done_height:

				// Add merged face
				int x = get_x(face,i,j,depth);
				int y = get_y(face,i,j,depth);
				int z = get_z(face,i,j,depth);
				int id = chunk->blocks[x][z][y]->block_id;
				
				add_face_to_mesh(mesh, x, y, z, face, id, w, h);

				// Mark region as processed
				for(int a = 0; a < w; a++) {
					for(int b = 0; b < h; b++) {
						mask[i + a][j + b] = false;
					}
				}
			}
		}
	}
}

void generate_chunk_mesh(chunk_t *chunk, mesh_t *mesh) {
	for(int face = 0; face < 6; face++) {
		greedy_mesh_face(chunk, mesh, face);
	}
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

chunk_t* get_chunk_at_position(chunk_t *chunks[128][128][64], Vector3 position) {
	int chunk_x = (int)(position.x / 16);
	int chunk_y = (int)(position.y / 16);
	int chunk_z = (int)(position.z / 16);
    
	if (chunk_x < 0 || chunk_x >= 128 || chunk_y < 0 || chunk_y >= 128 || chunk_z < 0 || chunk_z >= 64) {
	    return(NULL);
	}
    
	return chunks[chunk_x][chunk_y][chunk_z];
    }
    
voxel_t* get_voxel_at_position(chunk_t *chunk, Vector3 position) {
	if (!chunk)
		return(NULL);
    
	int voxel_x = (int)(position.x * 2.0f) % 32;
	int voxel_y = (int)(position.y * 2.0f) % 32;
	int voxel_z = (int)(position.z * 2.0f) % 32;
    
	return chunk->blocks[voxel_x][voxel_y][voxel_z];
}

//using dda
bool raycast(Ray ray, chunk_t *chunks[128][128][64]) {
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

bool	is_chunk_occluded(chunk_t *chunk, Vector3 camera_position, chunk_t *chunks[128][128][64]) {
	Vector3	chunk_center;
	Ray	ray;

	chunk_center = Vector3Add(chunk->bounding_box.min, chunk->bounding_box.max);
	chunk_center = Vector3Divide(chunk_center, (Vector3){0.5, 0.5, 0.5});	
	ray.direction = Vector3Normalize(Vector3Subtract(chunk_center, camera_position));
	ray.position = camera_position;
	
	return (raycast(ray, chunks));
}

void	extract_frustum_planes(Matrix view_proj, plane_t *frustum) {
	// Left plane
	frustum[0].normal.x = view_proj.m0 + view_proj.m3;
	frustum[0].normal.y = view_proj.m4 + view_proj.m7;
	frustum[0].normal.z = view_proj.m8 + view_proj.m11;
	frustum[0].distance = view_proj.m12 + view_proj.m15;
    
	// Right plane
	frustum[1].normal.x = -view_proj.m0 + view_proj.m3;
	frustum[1].normal.y = -view_proj.m4 + view_proj.m7;
	frustum[1].normal.z = -view_proj.m8 + view_proj.m11;
	frustum[1].distance = -view_proj.m12 + view_proj.m15;
    
	// Bottom plane
	frustum[2].normal.x = view_proj.m1 + view_proj.m3;
	frustum[2].normal.y = view_proj.m5 + view_proj.m7;
	frustum[2].normal.z = view_proj.m9 + view_proj.m11;
	frustum[2].distance = view_proj.m13 + view_proj.m15;
    
	// Top plane
	frustum[3].normal.x = -view_proj.m1 + view_proj.m3;
	frustum[3].normal.y = -view_proj.m5 + view_proj.m7;
	frustum[3].normal.z = -view_proj.m9 + view_proj.m11;
	frustum[3].distance = -view_proj.m13 + view_proj.m15;
    
	// Near plane
	frustum[4].normal.x = view_proj.m2 + view_proj.m3;
	frustum[4].normal.y = view_proj.m6 + view_proj.m7;
	frustum[4].normal.z = view_proj.m10 + view_proj.m11;
	frustum[4].distance = view_proj.m14 + view_proj.m15;
    
	// Far plane
	frustum[5].normal.x = -view_proj.m2 + view_proj.m3;
	frustum[5].normal.y = -view_proj.m6 + view_proj.m7;
	frustum[5].normal.z = -view_proj.m10 + view_proj.m11;
	frustum[5].distance = -view_proj.m14 + view_proj.m15;

	for (int i = 0; i < 6; i++) {
		float length = Vector3Length(frustum[i].normal);
		frustum[i].normal = Vector3Normalize(frustum[i].normal);
		frustum[i].distance /= length;
	}
}

void	gen_render_chunk(world_t *world, Camera3D *camera) {
	float cx = camera->position.x / 16;
	float cy = camera->position.y / 16;
	float cz = camera->position.z / 16;

	Vector3 forward = GetCameraForward(camera);

	Matrix view_proj = GetCameraProjectionMatrix(camera, GetRenderHeight()/ GetRenderWidth());

	plane_t frustum[6];
	extract_frustum_planes(view_proj, &frustum[0]);

	for (int x = 0; x < 128; x++) {
		for (int z = 0; z < 128; z++) {
			for (int y = 0; y < 64; y++) {
				chunk_t *current_chunk = world->chunk[x][z][y];
				if (is_chunk_visible(current_chunk, frustum) && !is_chunk_occluded(current_chunk, camera->position, world->chunk)) {
					add_chunk_to_render_list();
				}
			}
		}
	}
}

Texture2D	gen_texture_noise(fnl_state *noise) {
	Image test = GenImageColor(1024, 1024, BLACK);
	for (int x = 0; x < 1024; x++) {
		for (int z = 0; z < 1024; z++) {
			float noise_data = fnlGetNoise2D(noise, x, z) + 1;
			char color_data = floorf(noise_data * 255 / 2);
			Color tmp = {
				.a = 255,
				.r = color_data,
				.g = color_data,
				.b = color_data,
			};
			ImageDrawPixel(&test, x, z, tmp);
		}
	}

	Texture2D text =  LoadTextureFromImage(test);
	UnloadImage(test);
	return (text);
}

void	set_block(chunk_t *chunk, int x, int y, int z, voxel_t *vox) {
}
