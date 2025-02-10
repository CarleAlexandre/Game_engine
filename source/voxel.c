#include <engine.h>
#include <stdio.h>
#include <string.h>
#define FNL_IMPL
#include <FastNoiseLite.h>

void	generate_dungeon() {
	
}

void	load_chunk_data() {

}

void	unload_chunk_data() {

}

// void append_chunk_to_combined_mesh(combined_mesh_t *combined_mesh, mesh_t *chunk_mesh) {
// 	// Append vertex data
// 	for (int i = 0; i < chunk_mesh->vertex_count; i++) {
// 	    combined_mesh->vertices[combined_mesh->vertex_count++] = chunk_mesh->vertices[i];
// 	}
    
// 	// Append index data with offset
// 	unsigned int vertex_offset = combined_mesh->vertex_count - chunk_mesh->vertex_count;
// 	for (int i = 0; i < chunk_mesh->index_count; i++) {
// 	    combined_mesh->indices[combined_mesh->index_count++] = chunk_mesh->indices[i] + vertex_offset;
// 	}
// }

// void generate_combined_mesh(engine_t *engine, chunk_t *chunks, int chunk_num, combined_mesh_t *combined_mesh) {
// 	for (int i = 0; i < chunk_num; i++) {
// 	    mesh_t chunk_mesh = {0}; // Initialize chunk mesh
// 	    generate_chunk_mesh(engine, &chunks[i], &chunk_mesh); // Generate mesh for the chunk
// 	    append_chunk_to_combined_mesh(combined_mesh, &chunk_mesh); // Append to combined mesh
// 	}
// }

int	pack_vertex_data(int x, int y, int z, int face, char id, int extra) {
	return (x << 27) | (y << 22) | (z << 17) | (face << 14) | (id << 11) | extra;
}

void	unpack_vertex_data(int packed, int *x, int *y, int *z, int *face, char *id, int *extra) {
	*x = (packed >> 27) & 0x1F;    // Extract x (5 bits)
	*y = (packed >> 22) & 0x1F;    // Extract y (5 bits)
	*z = (packed >> 17) & 0x1F;    // Extract z (5 bits)
	*face = (packed >> 14) & 0x07; // Extract face (3 bits)
	*id = (packed >> 11) & 0x07; // block id (3 bits)
	*extra = packed & 0x7FF;      // Extract extra data (11 bits)
}

void	add_face_to_mesh(mesh_t *mesh, int x, int y, int z, int face, char id) {
	int vertex1, vertex2, vertex3, vertex4;

	switch (face) {
		case FACE_TOP:
			// Top face (y + 1), viewed from above
			vertex1 = pack_vertex_data(x,     y + 1, z,     face, id, 0);
			vertex2 = pack_vertex_data(x,     y + 1, z + 1, face, id, 0);
			vertex3 = pack_vertex_data(x + 1, y + 1, z + 1, face, id, 0);
			vertex4 = pack_vertex_data(x + 1, y + 1, z,     face, id, 0);
			break;

		case FACE_BOTTOM:
			// Bottom face (y), viewed from below
			vertex1 = pack_vertex_data(x,     y, z,     face, id, 0);
			vertex2 = pack_vertex_data(x + 1, y, z,     face, id, 0);
			vertex3 = pack_vertex_data(x + 1, y, z + 1, face, id, 0);
			vertex4 = pack_vertex_data(x,     y, z + 1, face, id, 0);
			break;

		case FACE_LEFT:
			// Left face (x), viewed from negative X direction
			vertex1 = pack_vertex_data(x, y,     z,     face, id, 0);
			vertex2 = pack_vertex_data(x, y,     z + 1, face, id, 0);
			vertex3 = pack_vertex_data(x, y + 1, z + 1, face, id, 0);
			vertex4 = pack_vertex_data(x, y + 1, z,     face, id, 0);
			break;

		case FACE_RIGHT:
			// Right face (x + 1), viewed from positive X direction
			vertex1 = pack_vertex_data(x + 1, y,     z,     face, id, 0);
			vertex2 = pack_vertex_data(x + 1, y + 1, z,     face, id, 0);
			vertex3 = pack_vertex_data(x + 1, y + 1, z + 1, face, id, 0);
			vertex4 = pack_vertex_data(x + 1, y,     z + 1, face, id, 0);
			break;

		case FACE_FRONT:
			// Front face (z + 1), viewed from positive Z direction
			vertex1 = pack_vertex_data(x,     y,     z + 1, face, id, 0);
			vertex2 = pack_vertex_data(x + 1, y,     z + 1, face, id, 0);
			vertex3 = pack_vertex_data(x + 1, y + 1, z + 1, face, id, 0);
			vertex4 = pack_vertex_data(x,     y + 1, z + 1, face, id, 0);
			break;

		case FACE_BACK:
			// Back face (z), viewed from negative Z direction
			vertex1 = pack_vertex_data(x,     y,     z,     face, id, 0);
			vertex2 = pack_vertex_data(x,     y + 1, z,     face, id, 0);
			vertex3 = pack_vertex_data(x + 1, y + 1, z,     face, id, 0);
			vertex4 = pack_vertex_data(x + 1, y,     z,     face, id, 0);
			break;

		default:
			return;
	}
    
	// Add vertices to the mesh
	mesh->vertices[mesh->vertex_count++] = vertex1;
	mesh->vertices[mesh->vertex_count++] = vertex2;
	mesh->vertices[mesh->vertex_count++] = vertex3;
	mesh->vertices[mesh->vertex_count++] = vertex4;
    
	// Add indices to the mesh (two triangles to form a quad)
	unsigned int base_index = mesh->vertex_count - 4;
	mesh->indices[mesh->index_count++] = base_index;
	mesh->indices[mesh->index_count++] = base_index + 1;
	mesh->indices[mesh->index_count++] = base_index + 2;
	mesh->indices[mesh->index_count++] = base_index;
	mesh->indices[mesh->index_count++] = base_index + 2;
	mesh->indices[mesh->index_count++] = base_index + 3;
}

chunk_t *generate_terrain(Vector2 chunk_pos) {
	// Create and configure noise state
	fnl_state noise = fnlCreateState();
	noise.noise_type = FNL_NOISE_OPENSIMPLEX2S;

	chunk_t *chunk = malloc(sizeof(chunk_t));
	memset(chunk->blocks, 0, sizeof(char[31][31][31]));

	for (int x = 0; x < 31; x++) {
		for (int z = 0; z < 31; z++) {
			float noise_data = (fnlGetNoise2D(&noise, x + chunk_pos.x * 31 , z + chunk_pos.y * 31) + 1) * 10;
			for (int y = 0; y < 31; y++) {
				if (y <= noise_data) {
					chunk->blocks[x][z][y] = 1;
				}
				if (y > noise_data && y < 5) {
					chunk->blocks[x][z][y] = 2;
				}
			}
		}
	}
	return (chunk);
}

#define CHECK_NEIGHBOR(axis, edge_cond, neighbor_ptr, neighbor_axis) \
    (axis edge_cond) ? (current->blocks[x][z][y] != 0) : \
    (neighbor_ptr ? neighbor_ptr->blocks[neighbor_axis][z][y] != 0 : false)
    
void	clear_chunk_mesh(chunk_t *chunk) {
	if (chunk->mesh.index_count) {
		memset(chunk->mesh.indices, 9, sizeof(int) * chunk->mesh.index_count);
		memset(chunk->mesh.vertices, 9, sizeof(int) * chunk->mesh.vertex_count);
		chunk->mesh.index_count = 0;
		chunk->mesh.vertex_count = 0;
	}
}

void	generate_chunk_mesh(chunk_t *chunk) {
	for (int x = 0; x < 31; x++) {
		for (int z = 0; z < 31; z++) {
			for (int y = 0; y < 31; y++) {
				char id = chunk->blocks[x][z][y];
				if (!id) continue;
				if (id == 2) {
					if (y == 30 || chunk->blocks[x][z][y + 1] != 2) {
						add_face_to_mesh(&chunk->trans, x, y, z, FACE_TOP, id);
					}
				} else {
					// Top face
					if (y == 30 || chunk->blocks[x][z][y + 1] != 1) {
						add_face_to_mesh(&chunk->mesh, x, y, z, FACE_TOP, id);
					}
					
					// Bottom face
					if (y == 0 || chunk->blocks[x][z][y - 1] != 1) {
						add_face_to_mesh(&chunk->mesh, x, y, z, FACE_BOTTOM, id);
					}
					
					// Left face (negative X)
					if (x == 0 || chunk->blocks[x - 1][z][y] != 1) {
						add_face_to_mesh(&chunk->mesh, x, y, z, FACE_LEFT, id);
					}
					
					// Right face (positive X)
					if (x == 30 || chunk->blocks[x + 1][z][y] != 1) {
						add_face_to_mesh(&chunk->mesh, x, y, z, FACE_RIGHT, id);
					}
					
					// Front face (positive Z)
					if (z == 30 || chunk->blocks[x][z + 1][y] != 1) {
						add_face_to_mesh(&chunk->mesh, x, y, z, FACE_FRONT, id);
					}
					
					// Back face (negative Z)
					if (z == 0 || chunk->blocks[x][z - 1][y] != 1) {
						add_face_to_mesh(&chunk->mesh, x, y, z, FACE_BACK, id);
					}
				}
			}
		}
	}
}

void	setup_chunk_buffers(chunk_t *chunk) {
	// Generate and bind the VAO
	glGenVertexArrays(1, &chunk->vao);
	glBindVertexArray(chunk->vao);
    
	// Generate and bind the VBO
	glGenBuffers(1, &chunk->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
	glBufferData(GL_ARRAY_BUFFER, chunk->mesh.vertex_count * sizeof(int), chunk->mesh.vertices, GL_DYNAMIC_DRAW);
    
	// Generate and bind the EBO
	glGenBuffers(1, &chunk->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunk->mesh.index_count * sizeof(unsigned int), chunk->mesh.indices, GL_DYNAMIC_DRAW);
    
	// Set up vertex attribute pointer
	glVertexAttribIPointer(0, 1, GL_INT, sizeof(int), (void*)0); // Packed data is an integer
	glEnableVertexAttribArray(0);
    
	// Unbind the VAO
	glBindVertexArray(0);
}

void	reload_chunk_buffers(chunk_t *chunk) {
	// Bind the VAO
	glBindVertexArray(chunk->vao);
    
	// Update the VBO with new vertex data
	glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, chunk->mesh.vertex_count * sizeof(int), chunk->mesh.vertices);
    
	// Update the EBO with new index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->ebo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, chunk->mesh.index_count * sizeof(unsigned int), chunk->mesh.indices);
    
	// Unbind the VAO
	glBindVertexArray(0);
}

void	render_vox_mesh(chunk_t *chunk) {

	Matrix matModel = MatrixIdentity();
	Matrix matView = rlGetMatrixModelview();
	Matrix matProjection = rlGetMatrixProjection();
	glUniform3f(glGetUniformLocation(chunk->shader.id, "world_pos"), chunk->world_pos.x, chunk->world_pos.y, chunk->world_pos.z);
	rlSetUniformMatrix(glGetUniformLocation(chunk->shader.id, "matView"), matView);
	rlSetUniformMatrix(glGetUniformLocation(chunk->shader.id, "matProjection"), matProjection);
	rlSetUniformMatrix(glGetUniformLocation(chunk->shader.id, "matModel"), matModel);

	glBindVertexArray(chunk->vao);
	glDrawElements(GL_TRIANGLES, chunk->mesh.index_count, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void setup_chunk_trans(chunk_t *chunk) {
	// Generate and bind the VAO
	glGenVertexArrays(1, &chunk->vao_trans);
	glBindVertexArray(chunk->vao_trans);
    
	// Generate and bind the VBO
	glGenBuffers(1, &chunk->vbo_trans);
	glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo_trans);
	glBufferData(GL_ARRAY_BUFFER, chunk->trans.vertex_count * sizeof(int), chunk->trans.vertices, GL_DYNAMIC_DRAW);
    
	// Generate and bind the EBO
	glGenBuffers(1, &chunk->ebo_trans);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->ebo_trans);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunk->trans.index_count * sizeof(unsigned int), chunk->trans.indices, GL_DYNAMIC_DRAW);
    
	// Set up vertex attribute pointer
	glVertexAttribIPointer(0, 1, GL_INT, sizeof(int), (void*)0); // Packed data is an integer
	glEnableVertexAttribArray(0);
    
	// Unbind the VAO
	glBindVertexArray(0);
}

void	reload_chunk_trans(chunk_t *chunk) {
	// Bind the VAO
	glBindVertexArray(chunk->vao_trans);
    
	// Update the VBO with new vertex data
	glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo_trans);
	glBufferSubData(GL_ARRAY_BUFFER, 0, chunk->trans.vertex_count * sizeof(int), chunk->trans.vertices);
    
	// Update the EBO with new index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->ebo_trans);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, chunk->trans.index_count * sizeof(unsigned int), chunk->trans.indices);
    
	// Unbind the VAO
	glBindVertexArray(0);
}

void	render_vox_trans(chunk_t *chunk) {

	Matrix matModel = MatrixIdentity();
	Matrix matView = rlGetMatrixModelview();
	Matrix matProjection = rlGetMatrixProjection();
	glUniform3f(glGetUniformLocation(chunk->shader.id, "world_pos"), chunk->world_pos.x, chunk->world_pos.y, chunk->world_pos.z);
	rlSetUniformMatrix(glGetUniformLocation(chunk->shader.id, "matView"), matView);
	rlSetUniformMatrix(glGetUniformLocation(chunk->shader.id, "matProjection"), matProjection);
	rlSetUniformMatrix(glGetUniformLocation(chunk->shader.id, "matModel"), matModel);

	glBindVertexArray(chunk->vao_trans);
	glDrawElements(GL_TRIANGLES, chunk->trans.index_count, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void	set_block(chunk_t *chunk, int x, int y, int z, int id) {
	chunk->blocks[x][z][y] = id;
	clear_chunk_mesh(chunk);
	generate_chunk_mesh(chunk);
	reload_chunk_buffers(chunk);
}
