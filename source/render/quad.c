#include "render_impl.h"

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
