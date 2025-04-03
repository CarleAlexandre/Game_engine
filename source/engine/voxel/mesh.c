#include "voxel.h"

static const float	quad_vertices[] = {
	0.0f, 0.0f, 0.0f,//bot_left
	0.5f, 0.0f, 0.0f,//bot_right
	0.0f, 0.5f, 0.0f,//top_left
	0.5f, 0.5f, 0.0f,//top_right
};

static const uint32_t	quad_indices[] = {
	0,//bot_left
	1,//bot_right
	2,//top_left
	3,//top_right
};

void	voxel_mesh_pack() {

}

voxel_chunk_render_queue*	voxel_render_queue_create() {
	voxel_chunk_render_queue* rqueue = malloc(sizeof(voxel_chunk_render_queue));
	assert(rqueue);

	rqueue->meshes = 0x00;

	rqueue->ssbo_data = 0x00;
	rqueue->count = 0;

	// rlEnableShader();

	glGenBuffers(1, &rqueue->ssbo);
	assert(rqueue->ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, rqueue->ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, rqueue->count, rqueue->ssbo_data, RL_STREAM_COPY);
	glClearBufferData(GL_SHADER_STORAGE_BUFFER, GL_R8UI, GL_RED_INTEGER, GL_UNSIGNED_BYTE, NULL);// Clear buffer data to 0
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	return (rqueue);
}

bool	voxel_render_queue_update(voxel_chunk_render_queue* rqueue) {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, rqueue->ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, rqueue->count, rqueue->ssbo_data, RL_STREAM_COPY);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

voxel_mesh*	voxel_mesh_create(void) {
	voxel_mesh* mesh = malloc(sizeof(voxel_mesh));
	assert(mesh);

	mesh->vao = rlLoadVertexArray();
	rlEnableVertexArray(mesh->vao);

	mesh->vbo = rlLoadVertexBuffer(quad_vertices, sizeof(quad_vertices), false);
	rlSetVertexAttribute(0, 3, RL_FLOAT, false, 3 *sizeof(float), 0);
	rlEnableVertexAttribute(0);

	mesh->ebo = rlLoadVertexBufferElement(quad_indices, sizeof(quad_indices), false);
	mesh->ibo = rlLoadVertexBuffer(mesh->faces_buffer, mesh->face_count * sizeof(int), true);

	rlSetVertexAttribute(1, 1, RL_FLOAT, false, sizeof(int), 0);
	rlEnableVertexAttribute(1);
	rlSetVertexAttributeDivisor(1, 1);

	// rlSetVertexAttribute(2, 1, RL_FLOAT, false, sizeof(int), offsetof(quad_data, element));
	// rlEnableVertexAttribute(2);
	// rlSetVertexAttributeDivisor(2, 1);

	rlDisableVertexArray();
	return (mesh);
}

void	voxel_mesh_update(voxel_mesh* mesh) {
	rlEnableVertexArray(mesh->vao);
	rlUpdateVertexBuffer(mesh->ibo, mesh->faces_buffer, mesh->face_count * sizeof(int), 0);
	rlDisableVertexArray();
}

void	voxel_render_chunks(const voxel_chunk_render_queue rqueue, Shader shader, Matrix transform) {
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
		// rlBindShaderBuffer();
		for (int i = 0; i < 0; i++) {
			rlEnableVertexArray(rqueue.meshes[i].vao);
			glDrawElementsInstanced(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0, rqueue.meshes[i].face_count);
		}
		rlDisableVertexArray();
	} rlDisableShader();
	rlSetMatrixModelview(matView);
	rlSetMatrixProjection(matProjection);
}