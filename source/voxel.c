#include <engine.h>
#include <stdio.h>
#include <string.h>
#define FNL_IMPL
#include <FastNoiseLite.h>

#define MAX_VERTICES 100000

#define FACE_TOP    0
#define FACE_BOTTOM 1
#define FACE_LEFT   2
#define FACE_RIGHT  3
#define FACE_FRONT  4
#define FACE_BACK   5

/*
| 31 - 27 | 26 - 22 | 21 - 17 | 16 - 14 | 13 - 0 |
|   x     |   y     |   z     |  face   |  extra |
*/

typedef	struct {
    int	vertices[MAX_VERTICES]; // Array to store packed vertex data
    unsigned int indices[MAX_VERTICES]; // Array to store indices
    int vertex_count; // Current number of vertices
    int index_count; // Current number of indices
}	mesh_t;

void	generate_dungeon() {
	
}

void	load_chunk_data() {

}

void	unload_chunk_data() {

}

void	add_block() {

}

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
	// Pack vertex data for the 4 vertices of the face
	int vertex1 = pack_vertex_data(x, y, z, face, 0); // Example: extra data = 0
	int vertex2 = pack_vertex_data(x + 1, y, z, face, 0);
	int vertex3 = pack_vertex_data(x + 1, y, z + 1, face, 0);
	int vertex4 = pack_vertex_data(x, y, z + 1, face, 0);
    
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

void	init_mesh_buffer(mesh_t *mesh) {
	unsigned int vbo, vao, ebo;

	// Generate buffers
	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	// Bind the VAO
	glBindVertexArray(vao);

	// Upload vertex data
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, mesh->vertex_count * sizeof(int), mesh->vertices, GL_STATIC_DRAW);

	// Upload index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh->index_count * sizeof(unsigned int), mesh->indices, GL_STATIC_DRAW);

	// Set up vertex attribute pointer
	glVertexAttribIPointer(0, 1, GL_INT, sizeof(int), (void*)0); // Packed data is an integer
	glEnableVertexAttribArray(0);

	// Unbind the VAO
	glBindVertexArray(0);
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

	// Generate buffers
	glGenVertexArrays(1, &chunk->vao);
	glGenBuffers(1, &chunk->vbo);
	glGenBuffers(1, &chunk->ebo);

	// Bind the VAO
	glBindVertexArray(chunk->vao);

	// Upload vertex data
	glBindBuffer(GL_ARRAY_BUFFER, chunk->vbo);
	glBufferData(GL_ARRAY_BUFFER, chunk->mesh.vertex_count * sizeof(int), chunk->mesh.vertices, GL_STATIC_DRAW);

	// Upload index data
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

void render_mesh(chunk_t *chunk) {
	glBindVertexArray(&chunk->vao);
	glDrawElements(GL_TRIANGLES, chunk->mesh.index_count, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}
