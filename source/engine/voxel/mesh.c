#include "voxel.h"

static const float	quad_vertices[] = {
	0.0f, 0.0f, 0.0f,
	0.5f, 0.0f, 0.0f,
	0.0f, 0.5f, 0.0f,
	0.5f, 0.5f, 0.0f,
};

static const uint32_t	quad_indices[] = {
	0, 1, 2, 3,
};

uint32_t	voxel_mesh_pack_face(uint8_t face, uint8_t x, uint8_t y, uint8_t z, uint8_t height, uint8_t width) {
	return (((face   & 0x7F) << 25) |  // 7 bits for face   (0-127) for marching cube
		((width  & 0x1F) << 20) |  // 5 bits for width  (0-31)
		((height & 0x1F) << 15) |  // 5 bits for height (0-31)
		((z      & 0x1F) << 10) |  // 5 bits for Z      (0-31)
		((y      & 0x1F) << 5)  |  // 5 bits for Y      (0-31)
		(x       & 0x1F));         // 5 bits for X      (0-31)
}

uint64_t	voxel_mesh_pack_tex(uint16_t tex_id, uint16_t material, uint32_t data) {
	return ((tex_id << 48) | (material << 32) | data);
}

void	voxel_update_render_data(voxel_chunk_render_queue *rqueue) {
	if (rqueue->draw_cmd) {
		free(rqueue->draw_cmd);
	}
	rqueue->draw_cmd = calloc(rqueue->chunk_count, sizeof(DrawElementsIndirectCommand));
	assert(rqueue->draw_cmd);

	if (rqueue->global_data) {
		free(rqueue->global_data);
	}
	rqueue->global_data = calloc(rqueue->face_count, sizeof(uint64_t));
	assert(rqueue->global_data);

	uint32_t offset = 0;

	for (int i = 0; i < rqueue->chunk_count; i++) {
		rqueue->draw_cmd[i] = (DrawElementsIndirectCommand){
			.count = 4,
			.instanceCount = rqueue->meshes[i].face_count,
			.firstIndex = 0,
			.baseVertex = 0,
			.baseInstance = rqueue->face_count,
		};
		memcpy(rqueue->global_data[offset], rqueue->meshes[i].faces_buffer, sizeof(uint64_t) * rqueue->meshes[i].face_count);
		offset += rqueue->meshes[i].face_count;
		rqueue->face_count = offset;
	}

	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, rqueue->ibo);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, rqueue->chunk_count * sizeof(DrawElementsIndirectCommand), rqueue->draw_cmd, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, rqueue->global_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uint64_t) * rqueue->face_count, rqueue->global_data, GL_DYNAMIC_DRAW);

	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

//need to sort chunk for transparency,
//also need to not upload face that are oposite to player pos from chunk relative pos (unless player is in chunk)
voxel_chunk_render_queue*	voxel_render_queue_create() {
	voxel_chunk_render_queue* rqueue = malloc(sizeof(voxel_chunk_render_queue));
	assert(rqueue);

	rqueue->ssbo_data = 0x00;
	rqueue->meshes = 0x00;
	rqueue->global_data = 0x00;
	rqueue->face_count = 0;
	rqueue->chunk_count = 0;
	rqueue->draw_cmd = 0;

	rqueue->vao = rlLoadVertexArray();
	rlEnableVertexArray(rqueue->vao);

	rqueue->vbo = rlLoadVertexBuffer(quad_vertices, sizeof(quad_vertices), false);
	rlSetVertexAttribute(0, 3, RL_FLOAT, false, 3 *sizeof(float), 0);
	rlEnableVertexAttribute(0);

	rqueue->ebo = rlLoadVertexBufferElement(quad_indices, sizeof(quad_indices), false);

	rqueue->ibo = rlLoadVertexBuffer(rqueue->draw_cmd, rqueue->chunk_count, true);

	rqueue->global_vbo = rlLoadVertexBuffer(rqueue->global_data, rqueue->face_count, true);
	
	rlSetVertexAttribute(1, 1, RL_FLOAT, false, sizeof(int64_t), 0);
	rlEnableVertexAttribute(1);
	rlSetVertexAttributeDivisor(1, 1);

	rlDisableVertexArray();
	// rlEnableShader();

	glGenBuffers(1, &rqueue->ssbo);
	assert(rqueue->ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, rqueue->ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, rqueue->chunk_count, rqueue->ssbo_data, RL_STREAM_COPY);
	glClearBufferData(GL_SHADER_STORAGE_BUFFER, GL_R8UI, GL_RED_INTEGER, GL_UNSIGNED_BYTE, NULL);// Clear buffer data to 0
	
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	return (rqueue);
}

bool	voxel_render_queue_update(voxel_chunk_render_queue* rqueue) {
	//load all face if update into rqueue->ibo_data

	rlEnableVertexArray(rqueue->vao);

	rlUpdateVertexBuffer(rqueue->global_vbo, rqueue->global_data, sizeof(uint64_t) * rqueue->face_count, 0);
	
	rlDisableVertexArray();
	
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, rqueue->ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, rqueue->chunk_count, rqueue->ssbo_data, RL_STREAM_COPY);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
	
}

voxel_mesh*	voxel_mesh_create(void) {
	voxel_mesh* mesh = malloc(sizeof(voxel_mesh));
	assert(mesh);

	mesh->face_count = 0;
	mesh->faces_buffer = 0;
	return (mesh);
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

		// rlBindShaderBuffer();
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, rqueue.ssbo);
		glMultiDrawElementsIndirect(GL_TRIANGLE_STRIP, GL_UNSIGNED_INT, rqueue.draw_cmd, rqueue.chunk_count, 0);
		// glMultiDrawElements(,);
		// for (int i = 0; i < 0; i++) {
		// 	rlEnableVertexArray(rqueue.meshes[i].vao);
		// 	glDrawElementsInstanced(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0, rqueue.meshes[i].face_count);
		// }
		// rlDisableVertexArray();
	} rlDisableShader();
	rlSetMatrixModelview(matView);
	rlSetMatrixProjection(matProjection);
}
