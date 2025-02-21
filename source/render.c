#include <prototype.h>

const float quad_vertices[] = {
	-0.5f, -0.5f, 0.0f,//bot_left
	 0.5f, -0.5f, 0.0f,//bot_right
	-0.5f,  0.5f, 0.0f,//top_left
	 0.5f,  0.5f, 0.0f//top_right
};

const uint32_t quad_indices[] = {0, 1, 2, 3};

void	assemble_world_mesh(dyn_array_t *render_queue, world_render_t *render) {
	unsigned int rqueue_size = render_queue->size;
	unsigned int total_size = 0;
	void *buffer;

	if (render->cmd) {
		free(render->cmd);
	}
	render->cmd = malloc(sizeof(deic_t) * rqueue_size);
	for (int i = 0; i < rqueue_size; i++) {
		chunk_t *current = dyn_get(render_queue->arena, i);
		render->cmd[i].count = 6;
		render->cmd[i].instanceCount = current->mesh.faces->size;
		render->cmd[i].firstIndex = 0;
		render->cmd[i].baseVertex = 0;
		render->cmd[i].baseInstance = total_size;
		total_size += current->mesh.faces->size;
		// copy all face from current .mesh into new buffer
	}
	buffer = malloc(sizeof(face_data_t) * total_size);
	assert(buffer);

	for (int i = 0; i < rqueue_size;) {
		chunk_t *current = dyn_get(render_queue->arena, i);
		memcpy(buffer, current->mesh.faces->arena, current->mesh.faces->size);
		i+= current->mesh.faces->size;
	}

	rlUpdateVertexBuffer(render->indirect_buffer, buffer, sizeof(face_data_t) * total_size, 0);
	rlUpdateShaderBuffer(render->storage_buffer, render->position_buffer, rqueue_size, 0);
}

world_render_t	gen_world_render(dyn_array_t *faces, Vector3 *pos, int num_chunk) {
	world_render_t render;

	render.vertex_array = rlLoadVertexArray();
	rlEnableVertexArray(render.vertex_array);

	render.vertex_buffer = rlLoadVertexBuffer(quad_vertices, sizeof(quad_vertices), false);
	rlSetVertexAttribute(0, 3, RL_FLOAT, false, 3 *sizeof(float), 0);
	rlEnableVertexAttribute(0);

	render.element_buffer = rlLoadVertexBufferElement(quad_indices, sizeof(quad_indices), false);

	render.indirect_buffer = rlLoadVertexBuffer(faces, (faces->size * sizeof(face_data_t)), true);

	rlSetVertexAttribute(1, 1, RL_FLOAT, false, sizeof(face_data_t), offsetof(face_data_t, face_data));
	rlEnableVertexAttribute(1);
	rlSetVertexAttributeDivisor(1, 1);

	rlSetVertexAttribute(2, 1, RL_FLOAT, false, sizeof(face_data_t), offsetof(face_data_t, block_id));
	rlEnableVertexAttribute(2);
	rlSetVertexAttributeDivisor(2, 1);

	rlDisableVertexArray();

	//ssbo

	render.storage_buffer = rlLoadShaderBuffer(num_chunk * sizeof(face_data_t), pos, RL_STREAM_COPY);
}

gbuffer_t	loadGbuffer(int width, int height, Shader deffered_shader) {
	gbuffer_t buffer = {0};

	buffer.framebuffer = rlLoadFramebuffer();
	if (!buffer.framebuffer) {
		TraceLog(LOG_WARNING, "failed to create framebuffer");
		exit(1);
	}
	rlEnableFramebuffer(buffer.framebuffer);
	buffer.positionTexture = rlLoadTexture(NULL, width,height, RL_PIXELFORMAT_UNCOMPRESSED_R32G32B32, 1);
	buffer.normalTexture = rlLoadTexture(NULL, width,height, RL_PIXELFORMAT_UNCOMPRESSED_R32G32B32, 1);
	buffer.albedoSpecTexture = rlLoadTexture(NULL, width,height, RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
	buffer.zTexture = rlLoadTexture(NULL, width,height, RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
	rlActiveDrawBuffers(4);

	rlFramebufferAttach(buffer.framebuffer, buffer.positionTexture, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_TEXTURE2D, 0);
	rlFramebufferAttach(buffer.framebuffer, buffer.normalTexture, RL_ATTACHMENT_COLOR_CHANNEL1, RL_ATTACHMENT_TEXTURE2D, 0);
	rlFramebufferAttach(buffer.framebuffer, buffer.albedoSpecTexture, RL_ATTACHMENT_COLOR_CHANNEL2, RL_ATTACHMENT_TEXTURE2D, 0);
	rlFramebufferAttach(buffer.framebuffer, buffer.zTexture, RL_ATTACHMENT_COLOR_CHANNEL3, RL_ATTACHMENT_TEXTURE2D, 0);

	buffer.depthRenderbuffer = rlLoadTextureDepth(width, height, true);
	rlFramebufferAttach(buffer.framebuffer, buffer.depthRenderbuffer, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_RENDERBUFFER, 0);

	if (!rlFramebufferComplete(buffer.framebuffer)) {
		TraceLog(LOG_WARNING, "Framebuffer is not complete");
		exit(1);
	}

	rlEnableShader(deffered_shader.id);
		rlSetUniformSampler(rlGetLocationUniform(deffered_shader.id, "gPosition"), 0);
		rlSetUniformSampler(rlGetLocationUniform(deffered_shader.id, "gNormal"), 1);
		rlSetUniformSampler(rlGetLocationUniform(deffered_shader.id, "gAlbedoSpec"), 2);
		rlSetUniformSampler(rlGetLocationUniform(deffered_shader.id, "gZ"), 3);
	rlDisableShader();

	rlEnableDepthTest();
	rlEnableBackfaceCulling();
	return (buffer);
}

void	voxel_render(engine_t *engine, world_t *world) {
	float camera_pos[3] = {engine->camera.position.x, engine->camera.position.y, engine->camera.position.z};
	int screen_height = GetScreenHeight(), screen_width = GetScreenWidth();

	if (IsKeyPressed(KEY_F3)) {
		printf("reloading world render!\n");
		for (int i = 0; i < sizeof(engine->shader) / sizeof(Shader); i++) {
			//reload shader here
		}
		printf("reloaded world render\n");
	}

	BeginDrawing();
	ClearBackground(BLACK);
	BeginMode3D(engine->camera);
		
		//opaque material render
		rlEnableShader(engine->shader[shader_voxel_solid].id);
		Matrix mvp = MatrixMultiply(MatrixMultiply(rlGetMatrixModelview(), rlGetMatrixProjection()),MatrixIdentity());
		rlSetUniformMatrix(engine->shader[SHADER_LOC_MATRIX_MVP].id, mvp);
		
		rlBindShaderBuffer(world->render.storage_buffer, 0);
		rlEnableVertexArray(world->render.vertex_array);

		rlBindShaderBuffer(world->render.storage_buffer, 0);

		glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, (const void*)0, world->render.render_queue->size, sizeof(deic_t));

		rlDisableVertexArray();
		rlDisableShader();

	if (engine->debug == true) {
		DrawBoundingBox(engine->player.bound, RED);
		DrawLine3D((Vector3){-100, 0, 0}, (Vector3){100, 0, 0}, RED);
		DrawLine3D((Vector3){0, -100, 0}, (Vector3){0, 100, 0}, GREEN);
		DrawLine3D((Vector3){0, 0, -100}, (Vector3){0, 0, 100}, BLUE);
	}
	EndMode3D();
	draw_ui(engine->player);
	EndDrawing();
}
