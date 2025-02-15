#define FNL_IMPL
#include "engine.h"

int	pack_face_data(char pos[3], char face, char height, char width) {
	// hhhh hwww wwff fyyy yyzz zzzx xxxx
	return (height << 23 | width << 18 | face << 15 | pos[2] << 10 | pos[1] << 5| pos[0]);
}

void	generate_dungeon() {
	
}

void	load_chunk_data() {

}

void	unload_chunk_data() {

}

// void	add_face_to_mesh(mesh_t *mesh, int x, int y, int z, int face, char id) {
// 	int vertex1, vertex2, vertex3, vertex4;

// 	switch (face) {
// 		case FACE_TOP:
// 			// Top face (y + 1), viewed from above
// 			vertex1 = pack_vertex_data(x,     y + 1, z,     face, id, 0);
// 			vertex2 = pack_vertex_data(x,     y + 1, z + 1, face, id, 0);
// 			vertex3 = pack_vertex_data(x + 1, y + 1, z + 1, face, id, 0);
// 			vertex4 = pack_vertex_data(x + 1, y + 1, z,     face, id, 0);
// 			break;

// 		case FACE_BOTTOM:
// 			// Bottom face (y), viewed from below
// 			vertex1 = pack_vertex_data(x,     y, z,     face, id, 0);
// 			vertex2 = pack_vertex_data(x + 1, y, z,     face, id, 0);
// 			vertex3 = pack_vertex_data(x + 1, y, z + 1, face, id, 0);
// 			vertex4 = pack_vertex_data(x,     y, z + 1, face, id, 0);
// 			break;

// 		case FACE_LEFT:
// 			// Left face (x), viewed from negative X direction
// 			vertex1 = pack_vertex_data(x, y,     z,     face, id, 0);
// 			vertex2 = pack_vertex_data(x, y,     z + 1, face, id, 0);
// 			vertex3 = pack_vertex_data(x, y + 1, z + 1, face, id, 0);
// 			vertex4 = pack_vertex_data(x, y + 1, z,     face, id, 0);
// 			break;

// 		case FACE_RIGHT:
// 			// Right face (x + 1), viewed from positive X direction
// 			vertex1 = pack_vertex_data(x + 1, y,     z,     face, id, 0);
// 			vertex2 = pack_vertex_data(x + 1, y + 1, z,     face, id, 0);
// 			vertex3 = pack_vertex_data(x + 1, y + 1, z + 1, face, id, 0);
// 			vertex4 = pack_vertex_data(x + 1, y,     z + 1, face, id, 0);
// 			break;

// 		case FACE_FRONT:
// 			// Front face (z + 1), viewed from positive Z direction
// 			vertex1 = pack_vertex_data(x,     y,     z + 1, face, id, 0);
// 			vertex2 = pack_vertex_data(x + 1, y,     z + 1, face, id, 0);
// 			vertex3 = pack_vertex_data(x + 1, y + 1, z + 1, face, id, 0);
// 			vertex4 = pack_vertex_data(x,     y + 1, z + 1, face, id, 0);
// 			break;

// 		case FACE_BACK:
// 			// Back face (z), viewed from negative Z direction
// 			vertex1 = pack_vertex_data(x,     y,     z,     face, id, 0);
// 			vertex2 = pack_vertex_data(x,     y + 1, z,     face, id, 0);
// 			vertex3 = pack_vertex_data(x + 1, y + 1, z,     face, id, 0);
// 			vertex4 = pack_vertex_data(x + 1, y,     z,     face, id, 0);
// 			break;

// 		default:
// 			return;
// 	}
    
// 	// Add vertices to the mesh
// 	mesh->vertices[mesh->vertex_count++] = vertex1;
// 	mesh->vertices[mesh->vertex_count++] = vertex2;
// 	mesh->vertices[mesh->vertex_count++] = vertex3;
// 	mesh->vertices[mesh->vertex_count++] = vertex4;
    
// 	// Add indices to the mesh (two triangles to form a quad)
// 	unsigned int base_index = mesh->vertex_count - 4;
// 	mesh->indices[mesh->index_count++] = base_index;
// 	mesh->indices[mesh->index_count++] = base_index + 1;
// 	mesh->indices[mesh->index_count++] = base_index + 2;
// 	mesh->indices[mesh->index_count++] = base_index;
// 	mesh->indices[mesh->index_count++] = base_index + 2;
// 	mesh->indices[mesh->index_count++] = base_index + 3;
// }


/*
	first generate terrain
	then change block type
	then generate caves
	then add minerals
	then generate hydrometrie
	then add erosion and water material displacement
	then add structures
*/

// world_t *generate_terrain(Vector2 chunk_pos) {
// 	// Create and configure noise state
// 	fnl_state noise = fnlCreateState();
// 	noise.noise_type = FNL_NOISE_OPENSIMPLEX2S;
// 	noise.cellular_distance_func = FNL_CELLULAR_DISTANCE_MANHATTAN;
// 	noise.fractal_type = FNL_FRACTAL_RIDGED;

// 	world_t *world = malloc(sizeof(world_t));

// 	world->chunk = init_svo(1024, 6);//64 chunk de 16 unite de long (32 block, 1 block = 0.5 units)


// 	Color *heightmap = malloc(sizeof(Color) * 1024 * 1024);

// 	//generate terrain heightmap:
// 	for (int x = 0; x < 1024; x++) {
// 		for (int z = 0; z < 1024; z++) {
// 			float noise_data = fnlGetNoise2D(&noise, x, z) + 1;
// 			//-1 1 1 = whith - 1 black
// 			char color_data = floorf(noise_data * 255 / 2);
// 			heightmap[x + z * 1024].a = 255;
// 			heightmap[x + z * 1024].r = color_data;
// 			heightmap[x + z * 1024].g = color_data;
// 			heightmap[x + z * 1024].b = color_data;
// 		}
// 	}

// 	return (world);
// }

Texture2D	gen_texture_noise(fnl_state *noise) {

	Image test = GenImageColor(1024, 1024, BLACK);
	//generate terrain heightmap:
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


// void	generate_chunk_mesh(chunk_t *chunk, vox_mesh_t mesh) {
// 	for (int x = 0; x < 31; x++) {
// 		for (int z = 0; z < 31; z++) {
// 			for (int y = 0; y < 31; y++) {
// 				int id = chunk->blocks[x][z][y]->block_id;
// 				if (!id) continue;
// 				if (id == 2) {
// 					if (y == 30 || chunk->blocks[x][z][y + 1]->block_id != 2) {
// 						add_face_to_mesh(&mesh, x, y, z, FACE_TOP, id);
// 					}
// 				} else {
// 					// Top face
// 					if (y == 30 || chunk->blocks[x][z][y + 1]->block_id != 1) {
// 						add_face_to_mesh(&mesh, x, y, z, FACE_TOP, id);
// 					}
					
// 					// Bottom face
// 					if (y == 0 || chunk->blocks[x][z][y - 1]->block_id != 1) {
// 						add_face_to_mesh(&mesh, x, y, z, FACE_BOTTOM, id);
// 					}
					
// 					// Left face (negative X)
// 					if (x == 0 || chunk->blocks[x - 1][z][y]->block_id != 1) {
// 						add_face_to_mesh(&mesh, x, y, z, FACE_LEFT, id);
// 					}
					
// 					// Right face (positive X)
// 					if (x == 30 || chunk->blocks[x + 1][z][y]->block_id != 1) {
// 						add_face_to_mesh(&mesh, x, y, z, FACE_RIGHT, id);
// 					}
					
// 					// Front face (positive Z)
// 					if (z == 30 || chunk->blocks[x][z + 1][y]->block_id != 1) {
// 						add_face_to_mesh(&mesh, x, y, z, FACE_FRONT, id);
// 					}
					
// 					// Back face (negative Z)
// 					if (z == 0 || chunk->blocks[x][z - 1][y]->block_id != 1) {
// 						add_face_to_mesh(&mesh, x, y, z, FACE_BACK, id);
// 					}
// 				}
// 			}
// 		}
// 	}
// }

// bool	is_chunk_visible(BoundingBox *bbox, const plane_t frustum[6]) {
// 	for (int i = 0; i < 6; i++) {
// 		Vector3	positive_corner = bbox->min;
// 		if (frustum[i].normal.x >= 0) {
// 			positive_corner.x = bbox->max.x;
// 		}
// 		if (frustum[i].normal.y >= 0) {
// 			positive_corner.y = bbox->max.y;
// 		}
// 		if (frustum[i].normal.z >= 0) {
// 			positive_corner.z = bbox->max.z;
// 		}
    
// 		float distance = Vector3DotProduct(frustum[i].normal, positive_corner) + frustum[i].distance;
// 		if (distance < 0) {
// 		    return(false);
// 		}
// 	}
// 	return(true);
// }

// void	svo_traversal(svo_t *svo, Ray ray) {

// }

// void	chunk_traversal(chunk_t *chunk, Ray ray) {

// }

// bool	raycast(Ray ray, svo_t *octree) {
// 	// Implement a voxel traversal algorithm to check for intersections
// 	// Return true if the ray hits a solid voxel before reaching the chunk
// }

// bool	is_chunk_occluded(chunk_t *chunk, Vector3 camera_position, svo_t *svo) {
// 	Vector3	chunk_center;
// 	Ray	ray;

// 	chunk_center = Vector3Add(chunk->bounding_box.min, chunk->bounding_box.max);
// 	chunk_center = Vector3Divide(chunk_center, (Vector3){0.5, 0.5, 0.5});	
// 	ray.direction = Vector3Normalize(Vector3Subtract(chunk_center, camera_position));
// 	ray.position = camera_position;
	
// 	return (raycast(ray, svo));
// }

// void	extract_frustum_planes(const Matrix view_proj, plane_t *frustum) {
	
// }

// void	gen_render_chunk(world_t *world, Camera3D *camera) {
// 	float cx = camera->position.x / 32;
// 	float cy = camera->position.y / 32;
// 	float cz = camera->position.z / 32;

// 	Vector3 forward = GetCameraForward(camera);

// 	Matrix view_proj = GetCameraProjectionMatrix(camera, GetRenderHeight()/ GetRenderWidth());

// 	plane_t frustum[6];
// 	extract_frustum_planes(view_proj, &frustum[0]);

// 	for (int i = 0; i <  world->chunk_count; i++) {
// 		chunk_t *chunk;
// 		if (is_chunk_visible(chunk, frustum) && !is_chunk_occluded(chunk, camera->position, world->chunk)) {
// 			add_chunk_to_render_list();
// 		}
// 	}
// }

void	set_block(chunk_t *chunk, int x, int y, int z, int id) {
	// chunk->blocks[x][z][y] = id;
	// clear_chunk_mesh(chunk);
	// generate_chunk_mesh(chunk);
	// reload_chunk_buffers(chunk);
}
