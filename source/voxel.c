#include <engine.h>
#include <stdio.h>
#include <string.h>
#define FNL_IMPL
#include <FastNoiseLite.h>


// #define MAX_COMBINED_VERTICES 1000000

// typedef struct {
//     int vertices[MAX_COMBINED_VERTICES];       // Combined vertex data for all chunks
//     unsigned int indices[MAX_COMBINED_VERTICES]; // Combined index data for all chunks
//     int vertex_count;                 // Total number of vertices
//     int index_count;                  // Total number of indices
// } combined_mesh_t;

void	generate_dungeon() {
	
}

void	load_chunk_data() {

}

void	unload_chunk_data() {

}

void	add_block() {

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

int	pack_vertex_data(int x, int y, int z, int face, int extra) {
	return (x << 27) | (y << 22) | (z << 17) | (face << 14) | extra;
}

void	unpack_vertex_data(int packed, int *x, int *y, int *z, int *face, int *extra) {
	*x = (packed >> 27) & 0x1F;    // Extract x (5 bits)
	*y = (packed >> 22) & 0x1F;    // Extract y (5 bits)
	*z = (packed >> 17) & 0x1F;    // Extract z (5 bits)
	*face = (packed >> 14) & 0x07; // Extract face (3 bits)
	*extra = packed & 0x3FFF;      // Extract extra data (14 bits)
}

void	add_face_to_mesh(mesh_t *mesh, int x, int y, int z, int face) {
	int vertex1, vertex2, vertex3, vertex4;

	switch (face) {
	    case FACE_TOP:
		// Top face (y + 1)
		vertex1 = pack_vertex_data(x,     y + 1, z,     face, 0);
		vertex2 = pack_vertex_data(x + 1, y + 1, z,     face, 0);
		vertex3 = pack_vertex_data(x + 1, y + 1, z + 1, face, 0);
		vertex4 = pack_vertex_data(x,     y + 1, z + 1, face, 0);
		break;
    
	    case FACE_BOTTOM:
		// Bottom face (y)
		vertex1 = pack_vertex_data(x,     y, z,     face, 0);
		vertex2 = pack_vertex_data(x + 1, y, z,     face, 0);
		vertex3 = pack_vertex_data(x + 1, y, z + 1, face, 0);
		vertex4 = pack_vertex_data(x,     y, z + 1, face, 0);
		break;
    
	    case FACE_LEFT:
		// Left face (z + 1)
		vertex1 = pack_vertex_data(x,     y,     z + 1, face, 0);
		vertex2 = pack_vertex_data(x + 1, y,     z + 1, face, 0);
		vertex3 = pack_vertex_data(x + 1, y + 1, z + 1, face, 0);
		vertex4 = pack_vertex_data(x,     y + 1, z + 1, face, 0);
		break;
    
	    case FACE_RIGHT:
		// Right face (z)
		vertex1 = pack_vertex_data(x,     y,     z, face, 0);
		vertex2 = pack_vertex_data(x + 1, y,     z, face, 0);
		vertex3 = pack_vertex_data(x + 1, y + 1, z, face, 0);
		vertex4 = pack_vertex_data(x,     y + 1, z, face, 0);
		break;
    
	    case FACE_FRONT:
		// Front face (x + 1)
		vertex1 = pack_vertex_data(x + 1, y,     z,     face, 0);
		vertex2 = pack_vertex_data(x + 1, y,     z + 1, face, 0);
		vertex3 = pack_vertex_data(x + 1, y + 1, z + 1, face, 0);
		vertex4 = pack_vertex_data(x + 1, y + 1, z,     face, 0);
		break;
    
	    case FACE_BACK:
		// Back face (x)
		vertex1 = pack_vertex_data(x, y,     z,     face, 0);
		vertex2 = pack_vertex_data(x, y,     z + 1, face, 0);
		vertex3 = pack_vertex_data(x, y + 1, z + 1, face, 0);
		vertex4 = pack_vertex_data(x, y + 1, z,     face, 0);
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
	memset(chunk, 0, sizeof(chunk_t));

	for (int x = 0; x < 32; x++) {
		for (int z = 0; z < 32; z++) {
			float noise_data = fnlGetNoise2D(&noise, x + chunk_pos.x *32 , z + chunk_pos.y * 32) * 2;
			for (int y = 0; y < noise_data; y++) {
				chunk->blocks[x][z][y] = true;
			}
		}
	}
	return (chunk);
}

void	generate_chunk_mesh(engine_t *engine, chunk_t *chunk) {
	for (int x = 0; x < 32; x++) {
		for (int z = 0; z < 32; z++) {
			for (int y = 0; y < 32; y++) {
				if (chunk->blocks[x][z][y]) {
					// Top face
					if (y < 31 && !chunk->blocks[x][z][y + 1]) {
						add_face_to_mesh(&chunk->mesh, x, y, z, FACE_TOP);
					}

					// Bottom face
					if (y > 0 && !chunk->blocks[x][z][y - 1]) {
						add_face_to_mesh(&chunk->mesh, x, y, z, FACE_BOTTOM);
					}

					// Left face
					if (z < 31 && !chunk->blocks[x][z + 1][y]) {
						add_face_to_mesh(&chunk->mesh, x, y, z, FACE_LEFT);
					}

					// Right face
					if (z > 0 && !chunk->blocks[x][z - 1][y]) {
						add_face_to_mesh(&chunk->mesh, x, y, z, FACE_RIGHT);
					}

					// Front face
					if (x < 31 && !chunk->blocks[x + 1][z][y]) {
						add_face_to_mesh(&chunk->mesh, x, y, z, FACE_FRONT);
					}

					// Back face
					if (x > 0 && !chunk->blocks[x - 1][z][y]) {
						add_face_to_mesh(&chunk->mesh, x, y, z, FACE_BACK);
					}
				}
			}
		}
	}
}

void setup_chunk_buffers(chunk_t *chunk) {
	// Generate and bind the VAO
	glGenVertexArrays(1, &chunk->vao);
	glBindVertexArray(chunk->vao);
    
	// Generate and bind the VBO
	glGenBuffers(1, &chunk->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
	glBufferData(GL_ARRAY_BUFFER, chunk->mesh.vertex_count * sizeof(int), chunk->mesh.vertices, GL_STATIC_DRAW);
    
	// Generate and bind the EBO
	glGenBuffers(1, &chunk->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, chunk->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, chunk->mesh.index_count * sizeof(unsigned int), chunk->mesh.indices, GL_STATIC_DRAW);
    
	// Set up vertex attribute pointer
	glVertexAttribIPointer(0, 1, GL_INT, sizeof(int), (void*)0); // Packed data is an integer
	glEnableVertexAttribArray(0);
    
	// Unbind the VAO
	glBindVertexArray(0);
}

void	combine_chunk_data(){

}

void render_vox_mesh(chunk_t *chunk) {

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
