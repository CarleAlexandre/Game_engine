#include "render_impl.h"

#define CHUNK_RESOLUTION 32
#define CHUNK_SIZE (CHUNK_RESOLUTION * CHUNK_RESOLUTION * CHUNK_RESOLUTION)
#define FACES 6

typedef struct chunk_texture {
	Texture2D texture[CHUNK_SIZE];
}	chunk_texture;

typedef struct voxel_data {
	uint16_t block_id;

}	voxel_data;

typedef struct voxel_chunk {
	voxel_data	voxel[CHUNK_SIZE];
	Image		voxel_bit;
	BoundingBox	box;
	Vector3		pos;
}	voxel_chunk;


void	voxel_texture_write() {

}

Image	voxel_binary_image_create(void *data) {
	Image image = {	
		.data = data,
		.width = CHUNK_RESOLUTION,
		.height = CHUNK_RESOLUTION,
		.format = PIXELFORMAT_UNCOMPRESSED_R32,
		.mipmaps = 1,
	};

	return (image);
}

void	voxel_chunk_debug_draw(const voxel_chunk *chunk) {
	//draw boundaries
	DrawCubeWires(chunk->pos, 32, 32, 32, RED);
	
}

void	voxel_render_quad_elements_instanced(float pos[3], Shader shader) {	
	SetShaderValue(shader, GetShaderLocation(shader, "chunk_pos"), &pos[0], RL_SHADER_UNIFORM_VEC3);
	
	rlEnableVertexArray(0);
		// int quad_count;
		// glDrawElementsInstanced(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0, quad_count);
	rlDisableVertexArray();
}

void	voxel_render_chunks(Shader shader, Matrix transform) {
	Matrix matModel = MatrixIdentity();
	Matrix matView = rlGetMatrixModelview();
	Matrix matProjection = rlGetMatrixProjection();
	rlEnableShader(shader.id ); {
		matModel = MatrixMultiply(transform, rlGetMatrixTransform());
		if (shader.locs[SHADER_LOC_MATRIX_MODEL] != -1) {
			rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_MODEL], matModel);
		}
		if (shader.locs[SHADER_LOC_MATRIX_VIEW] != -1) {
			rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_VIEW], matView);
		}
		if (shader.locs[SHADER_LOC_MATRIX_PROJECTION] != -1) {
			rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_PROJECTION], matProjection);
		}
		if (shader.locs[SHADER_LOC_MATRIX_NORMAL] != -1) {
			rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_NORMAL], MatrixTranspose(MatrixInvert(matModel)));
		}
		rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_MVP], MatrixMultiply(MatrixMultiply(matModel, matView), matProjection));
		for (int i = 0; i < 0; i++) {
			float pos[3];
			voxel_render_quad_elements_instanced(pos, shader);
		}
	} rlDisableShader();
	rlSetMatrixModelview(matView);
	rlSetMatrixProjection(matProjection);
}

quad_data haven_quad_pack(unsigned char pos[3], unsigned char face, unsigned char height, unsigned char width, unsigned short id) {
	quad_data data;
	data.render = false;
	data.element = (id << 3 | face);
	data.face_data = ((height << 24) | (width << 18) | (pos[2] << 12) | (pos[1] << 6) | pos[0]);
	return (data);
}

void	haven_quad_mesh(voxel_mesh *mesh) {
	int total_size = 0;

	for (int i = 0; i < 6; i++) {
		total_size += mesh->faces_count[i];
	}

	quad_data *buffer = calloc(total_size, sizeof(quad_data));

	int prev = 0;

	for (int i = 0; i < 6; i++) {
		memcpy(&buffer[prev], mesh->faces[i], mesh->faces_count[i] * sizeof(quad_data));
		prev += mesh->faces_count[i];
	}

	mesh->vao = rlLoadVertexArray();
	rlEnableVertexArray(mesh->vao);

	mesh->vbo = rlLoadVertexBuffer(quad_vertices, sizeof(quad_vertices), false);
	rlSetVertexAttribute(0, 3, RL_FLOAT, false, 3 *sizeof(float), 0);
	rlEnableVertexAttribute(0);

	mesh->ebo = rlLoadVertexBufferElement(quad_indices, sizeof(quad_indices), false);
	mesh->ibo = rlLoadVertexBuffer(buffer, total_size * sizeof(quad_data), true);

	rlSetVertexAttribute(1, 1, RL_FLOAT, false, sizeof(quad_data), offsetof(quad_data, face_data));
	rlEnableVertexAttribute(1);
	rlSetVertexAttributeDivisor(1, 1);

	rlSetVertexAttribute(2, 1, RL_FLOAT, false, sizeof(quad_data), offsetof(quad_data, element));
	rlEnableVertexAttribute(2);
	rlSetVertexAttributeDivisor(2, 1);

	rlDisableVertexArray();
}
