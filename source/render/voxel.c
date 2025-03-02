#include <engine.h>

static const float	quad_vertices[] = {
	0.0f, 0.0f, 0.0f, //bot_left
	0.5f, 0.0f, 0.0f,//bot_right
	0.0f, 0.5f, 0.0f,//top_left
	0.5f, 0.5f, 0.0f //top_right
};

static const uint32_t	quad_indices[] = {0, 1, 2, 3};

face_data_t	pack_face_data(unsigned char pos[3], unsigned char face, unsigned char height, unsigned char width, unsigned short id) {
	face_data_t data;

	data.block_id = (id << 3 | face);
	data.face_data = ((height << 24) | (width << 18) | (pos[2] << 12) | (pos[1] << 6) | pos[0]);
	return (data);
}

bool	is_chunk_visible(Vector3 chunk_pos, Camera camera) {
	Matrix view = GetCameraViewMatrix(&camera);
	Matrix proj = GetCameraProjectionMatrix(&camera, GetScreenHeight() / GetScreenWidth());
	Matrix view_proj = MatrixMultiply(view, proj);

	Frustum frustum;
	extract_frustum_from_matrix(view_proj, &frustum);

	BoundingBox chunk_box = {
		.min = chunk_pos,
		.max = {chunk_pos.x + CHUNK_SIZE * VOXEL_SIZE, chunk_pos.y + CHUNK_SIZE * VOXEL_SIZE, chunk_pos.z + CHUNK_SIZE * VOXEL_SIZE}
	};
	
	return is_box_in_frustum(chunk_box, frustum);
}


void	gen_chunk_mesh(chunk_t *current, world_t *world) {
	for (char x = 0; x < CHUNK_SIZE; x++) {
		for (char z = 0; z < CHUNK_SIZE; z++) {
			for (char y = 0; y < CHUNK_SIZE; y++) {
				Vector3 pos = {x, y, z};
				
				svo_node_t *node = svo_get_node(pos, current->blocks);
				
				if (!is_node_valid(node)) continue;
				
				for (int i = 0; i < 6; i++) {
					svo_node_t *neighbor = get_neighbor_block(pos, i, current, world);
					
					if (!is_node_valid(neighbor)) {
						char packed_pos[3] = {x, y, z};
						face_data_t face = pack_face_data(packed_pos, i, 1, 1, 1);
						
						if (current->mesh->faces_count[i] < 4096) {
							current->mesh->faces[i][current->mesh->faces_count[i]++] = face;
						}
					}
				}
			}
		}
	}
}

void	update_chunk_mesh(Vector3 pos, world_t *world) {
	svo_node_t *node = svo_get_node(pos, world->chunks);
	if (is_node_valid(node)) {
		chunk_t *current_chunk = node->data;
		if (!current_chunk->mesh) {
			current_chunk->mesh = calloc(1, sizeof(chunk_mesh_t));
		} else {
			for (int i = 0; i < 6; i++) {
				memset(&current_chunk->mesh->faces[i][0], 0, sizeof(face_data_t) * 4096);
				current_chunk->mesh->faces_count[i] = 0;
			}
		}
		gen_chunk_mesh(current_chunk, world);
	}
}

void	gen_world_mesh(world_t *world, engine_t *engine) {
	for (int x = 0; x < 8; x++) {
		for (int z = 0; z < 8; z++) {
			for (int y = 0; y < 8; y++) {
				svo_node_t *node = svo_get_node((Vector3){x,y,z}, world->chunks);
				if (is_node_valid(node)) {
					chunk_t *current_chunk = node->data;
					current_chunk->mesh = calloc(1, sizeof(chunk_mesh_t));
					gen_chunk_mesh(current_chunk, world);
				}
			}
		}
	}
}

//check this every few frame;
// also need to change data order at opposite from player to player pos, and skip not visible face when assembline the buffer
void	update_world_render(world_t *world, engine_t *engine) {
	if (world->rcount) {
		for (int i = 0; i < 512; i++) {
			world->rqueue[i] = NULL;
		}
		world->rcount = 0;
	}
	for (int x = 0; x < 8; x++) {
		for (int z = 0; z < 8; z++) {
			for (int y = 0; y < 8; y++) {
				svo_node_t *node = svo_get_node((Vector3){x,y,z}, world->chunks);
				if (is_node_valid(node)) {
					chunk_t *current_chunk = node->data;
					if (is_chunk_visible(current_chunk->pos, engine->camera) && current_chunk->mesh->faces_count) {
						world->rqueue[world->rcount++] = current_chunk;
					}
				}
			}
		}
	}
}

void	setup_world_render(world_t *world, engine_t *engine) {
	if (world->rcount) {
		for (int i = 0; i < 512; i++) {
			world->rqueue[i] = NULL;
		}
		world->rcount = 0;
	}
	for (int x = 0; x < 8; x++) {
		for (int z = 0; z < 8; z++) {
			for (int y = 0; y < 8; y++) {
				svo_node_t *node = svo_get_node((Vector3){x,y,z}, world->chunks);
				if (is_node_valid(node)) {
					chunk_t *current_chunk = node->data;
					world->rqueue[world->rcount++] = current_chunk;
				}
			}
		}
	}
}

world_mesh_t	assemble_world_mesh(chunk_t *rqueue[512], unsigned int rcount) {
	// face_data_t	faces[10000000] = {0};
	// unsigned int	face_count = 0;
	// Vector3		pos[512] = {0};
	// unsigned int	pos_count = 0;

	// for (int i = 0; i < rcount; i++) {
	// 	for (int k = 0; k < rqueue[i]->mesh->faces_count; k++) {
	// 		faces[face_count++] = rqueue[i]->mesh->faces[k];
	// 	}
	// 	pos[pos_count++] = rqueue[i]->pos;
	// }

	// world_mesh_t mesh = {0};

	// mesh.faces = malloc(sizeof(face_data_t) * face_count);
	// mesh.faces_count = face_count;
	// mesh.pos = malloc(sizeof(Vector3) * pos_count);
	// mesh.pos_count = pos_count;
}

world_render_t	gen_world_render(world_mesh_t *mesh) {
	world_render_t render;

	glGenVertexArrays(1, &render.vao);
	glBindVertexArray(render.vao);

	glGenBuffers(1, &render.vbo);
	glBindBuffer(GL_ARRAY_BUFFER, render.vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 3 *sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);

	glGenBuffers(1, &render.ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, render.ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_indices), quad_indices, GL_STATIC_DRAW);


	glGenBuffers(1, &render.ibo);
	glBindBuffer(GL_ARRAY_BUFFER, render.ibo);
	glBufferData(GL_ARRAY_BUFFER, 0, 0, GL_DYNAMIC_DRAW);

	rlSetVertexAttribute(1, 1, RL_FLOAT, false, sizeof(face_data_t), offsetof(face_data_t, face_data));
	rlEnableVertexAttribute(1);
	rlSetVertexAttributeDivisor(1, 1);

	rlSetVertexAttribute(2, 1, RL_FLOAT, false, sizeof(face_data_t), offsetof(face_data_t, block_id));
	rlEnableVertexAttribute(2);
	rlSetVertexAttributeDivisor(2, 1);

	glBindVertexArray(0);

	//ssbo
	glCreateBuffers(1, &render.ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, render.ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 0, 0, GL_STREAM_COPY);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	return (render);
}

// void	update_world_render(uint32_t id, void *queue) {
// 	glBindBuffer(GL_ARRAY_BUFFER, id);
// 	glBufferSubData(GL_ARRAY_BUFFER, 0, 0, 0);
// }

void	update_chunk_render(chunk_mesh_t *mesh) {
	int total_size = 0;

	for (int i = 0; i < 6; i++) {
		total_size += mesh->faces_count[i];
	}

	face_data_t *buffer = calloc(total_size, sizeof(face_data_t));

	int prev = 0;

	for (int i = 0; i < 6; i++) {
		memcpy(&buffer[prev], mesh->faces[i], mesh->faces_count[i] * sizeof(face_data_t));
		prev += mesh->faces_count[i];
	}

	glBindBuffer(GL_ARRAY_BUFFER, mesh->ibo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, total_size * sizeof(face_data_t), buffer);

}

void	gen_chunk_render(chunk_mesh_t *mesh) {
	int total_size = 0;

	for (int i = 0; i < 6; i++) {
		total_size += mesh->faces_count[i];
	}

	face_data_t *buffer = calloc(total_size, sizeof(face_data_t));

	int prev = 0;

	for (int i = 0; i < 6; i++) {
		memcpy(&buffer[prev], mesh->faces[i], mesh->faces_count[i] * sizeof(face_data_t));
		prev += mesh->faces_count[i];
	}

	mesh->vao = rlLoadVertexArray();
	rlEnableVertexArray(mesh->vao);

	mesh->vbo = rlLoadVertexBuffer(quad_vertices, sizeof(quad_vertices), false);
	rlSetVertexAttribute(0, 3, RL_FLOAT, false, 3 *sizeof(float), 0);
	rlEnableVertexAttribute(0);

	mesh->ebo = rlLoadVertexBufferElement(quad_indices, sizeof(quad_indices), false);

	mesh->ibo = rlLoadVertexBuffer(buffer, total_size * sizeof(face_data_t), true);

	rlSetVertexAttribute(1, 1, RL_FLOAT, false, sizeof(face_data_t), offsetof(face_data_t, face_data));
	rlEnableVertexAttribute(1);
	rlSetVertexAttributeDivisor(1, 1);

	rlSetVertexAttribute(2, 1, RL_FLOAT, false, sizeof(face_data_t), offsetof(face_data_t, block_id));
	rlEnableVertexAttribute(2);
	rlSetVertexAttributeDivisor(2, 1);

	rlDisableVertexArray();
}

void	render_voxel_work(Shader shader, Matrix transform, world_t *world) {
	rlEnableShader(shader.id );

	Matrix matModel = MatrixIdentity();
	Matrix matView = rlGetMatrixModelview();
	Matrix matModelView = MatrixIdentity();
	Matrix matProjection = rlGetMatrixProjection();

	if (shader.locs[SHADER_LOC_MATRIX_VIEW] != -1) rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_VIEW], matView);
	if (shader.locs[SHADER_LOC_MATRIX_PROJECTION] != -1) rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_PROJECTION], matProjection);
	
	matModel = MatrixMultiply(transform, rlGetMatrixTransform());

	if (shader.locs[SHADER_LOC_MATRIX_MODEL] != -1) rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_MODEL], matModel);

	matModelView = MatrixMultiply(matModel, matView);

	if (shader.locs[SHADER_LOC_MATRIX_NORMAL] != -1) rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_NORMAL], MatrixTranspose(MatrixInvert(matModel)));

	Matrix matModelViewProjection = MatrixIdentity();
	matModelViewProjection = MatrixMultiply(matModelView, matProjection);

	rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_MVP], matModelViewProjection);

	// glMultiDrawArrays(GL_TRIANGLE_STRIP, 0, 0, 0);
	for (int i = 0; i < world->rcount; i++) {
		chunk_t *chk = world->rqueue[i];
		float pos[3] = {chk->pos.x, chk->pos.y, chk->pos.z};
		
		SetShaderValue(shader, GetShaderLocation(shader, "chunk_pos"), &pos[0], RL_SHADER_UNIFORM_VEC3);

		rlEnableVertexArray(chk->mesh->vao);

		int total = 0;
		for (int i = 0; i < 6; i++) {
			total += chk->mesh->faces_count[i];
		}
		
		glDrawElementsInstanced(GL_TRIANGLE_STRIP, 4, GL_UNSIGNED_INT, 0, total);

		rlDisableVertexArray();
	}

	rlDisableShader();

	rlSetMatrixModelview(matView);
	rlSetMatrixProjection(matProjection);
}

void	voxel_render(engine_t *engine, world_t *world) {
	float camera_pos[3] = {engine->camera.position.x, engine->camera.position.y, engine->camera.position.z};
	int screen_height = GetScreenHeight(), screen_width = GetScreenWidth();

	BeginDrawing();
	ClearBackground(BLACK);
	
	BeginMode3D(engine->camera);
		rlDisableBackfaceCulling();
		rlDisableDepthMask();
		DrawModel(engine->skybox, (Vector3){0, 0, 0}, 1.0f, WHITE);
		rlEnableBackfaceCulling();
		rlEnableDepthMask();	
	
		render_voxel_work(engine->shader[shader_voxel_solid], MatrixIdentity(), world);

		Vector3 pos = {0};
		bool ret = voxel_look_at(engine->camera, world, 5, &pos);
		if (ret) {
			DrawCubeWires(pos, 0.5, 0.5, 0.5, BLACK);
		}

		// if (engine->debug == true) {
		// 	DrawBoundingBox(engine->player.bound, RED);
		// 	for (int i = 0; i <= 8; i++) {
		// 		for (int k = 0; k <= 8; k++) {
		// 			DrawLine3D((Vector3){0, i * 32, k * 32}, (Vector3){256,i * 32, k * 32}, RED);
		// 			DrawLine3D((Vector3){i * 32, 0, k * 32}, (Vector3){i * 32, 256, k * 32}, GREEN);
		// 			DrawLine3D((Vector3){i * 32, k * 32, 0}, (Vector3){i * 32, k * 32, 256}, BLUE);
		// 		}
		// 	}
		// }
	EndMode3D();
	draw_ui(engine->player);
	Vector3 forward = GetCameraForward(&engine->camera);
	DrawText(TextFormat("x:%.1f y:%.1f z:%.1f\n", forward.x, forward.y, forward.z), 10, 200, 20, GREEN);
	DrawText(TextFormat("x:%.0f y:%.0f z:%.0f\n", engine->camera.position.x, engine->camera.position.y, engine->camera.position.z), 10, 300, 20, GREEN);
	EndDrawing();
}
