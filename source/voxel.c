#include <engine.h>
#include <stdint.h>
#include <string.h>
#define FNL_IMPL
#include <FastNoiseLite.h>

uint64_t	pack_face_data(char pos[3], char face, char id, char height, char width, char extra) {
	//0000 0000 0000 0000 0000 hhhh hwww wwii iiii iiff fyyy yyzz zzzx xxxx
	return ((uint64_t)extra << 35 | height << 30 | width << 25 |  id << 18 | face << 15 | pos[2] << 10 | pos[1] << 5| pos[0]);
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

// 	world_t *world = malloc(sizeof(world_t));

// 	for (int x = 0; x < 31; x++) {
// 		for (int z = 0; z < 31; z++) {
// 			float noise_data = (fnlGetNoise2D(&noise, x + chunk_pos.x * 31 , z + chunk_pos.y * 31) + 1) * 10;
// 			for (int y = 0; y < 31; y++) {
// 				if (y <= noise_data) {
// 					chunk->blocks[x][z][y]->block_id = 1;
// 				}
// 			}
// 		}
// 	}

// 	return (world);
// }

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

void	gen_render_chunk(world_t *world, Vector3 player_pos) {
	float cx = player_pos.x / 32;
	float cy = player_pos.y / 32;
	float cz = player_pos.z / 32;

	world->render_chunk[0] = svo_get_node((float[3]){cx, cy, cz}, world->chunk)->data;

	world->render_chunk[1] = svo_get_node((float[3]){cx, cy + 1, cz}, world->chunk)->data;
	world->render_chunk[2] = svo_get_node((float[3]){cx, cy - 1, cz}, world->chunk)->data;
	world->render_chunk[3] = svo_get_node((float[3]){cx + 1, cy, cz}, world->chunk)->data;
	world->render_chunk[4] = svo_get_node((float[3]){cx - 1, cy, cz}, world->chunk)->data;
	world->render_chunk[5] = svo_get_node((float[3]){cx, cy, cz + 1}, world->chunk)->data;
	world->render_chunk[6] = svo_get_node((float[3]){cx, cy, cz - 1}, world->chunk)->data;

	world->render_chunk[7] = svo_get_node((float[3]){cx, cy + 1, cz + 1}, world->chunk)->data;
	world->render_chunk[8] = svo_get_node((float[3]){cx, cy + 1, cz - 1}, world->chunk)->data;
	world->render_chunk[9] = svo_get_node((float[3]){cx + 1, cy + 1, cz}, world->chunk)->data;
	world->render_chunk[10] = svo_get_node((float[3]){cx - 1, cy + 1, cz}, world->chunk)->data;

	world->render_chunk[11] = svo_get_node((float[3]){cx + 1, cy + 1, cz + 1}, world->chunk)->data;
	world->render_chunk[12] = svo_get_node((float[3]){cx + 1, cy + 1, cz - 1}, world->chunk)->data;
	world->render_chunk[13] = svo_get_node((float[3]){cx - 1, cy + 1, cz + 1}, world->chunk)->data;
	world->render_chunk[14] = svo_get_node((float[3]){cx - 1, cy + 1, cz - 1}, world->chunk)->data;

	world->render_chunk[15] = svo_get_node((float[3]){cx, cy - 1, cz + 1}, world->chunk)->data;
	world->render_chunk[16] = svo_get_node((float[3]){cx, cy - 1, cz - 1}, world->chunk)->data;
	world->render_chunk[17] = svo_get_node((float[3]){cx + 1, cy - 1, cz}, world->chunk)->data;
	world->render_chunk[18] = svo_get_node((float[3]){cx - 1, cy - 1, cz}, world->chunk)->data;

	world->render_chunk[19] = svo_get_node((float[3]){cx + 1, cy - 1, cz + 1}, world->chunk)->data;
	world->render_chunk[20] = svo_get_node((float[3]){cx + 1, cy - 1, cz - 1}, world->chunk)->data;
	world->render_chunk[21] = svo_get_node((float[3]){cx - 1, cy - 1, cz + 1}, world->chunk)->data;
	world->render_chunk[22] = svo_get_node((float[3]){cx - 1, cy - 1, cz - 1}, world->chunk)->data;

	world->render_chunk[23] = svo_get_node((float[3]){cx + 1, cy, cz + 1}, world->chunk)->data;
	world->render_chunk[24] = svo_get_node((float[3]){cx - 1, cy, cz + 1}, world->chunk)->data;
	world->render_chunk[25] = svo_get_node((float[3]){cx + 1, cy, cz - 1}, world->chunk)->data;
	world->render_chunk[26] = svo_get_node((float[3]){cx + 1, cy, cz - 1}, world->chunk)->data;

}

void	set_block(chunk_t *chunk, int x, int y, int z, int id) {
	// chunk->blocks[x][z][y] = id;
	// clear_chunk_mesh(chunk);
	// generate_chunk_mesh(chunk);
	// reload_chunk_buffers(chunk);
}
