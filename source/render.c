#include <prototype.h>

const float quad_vertices[] = {
	-0.5f, -0.5f, 0.0f,//bot_left
	 0.5f, -0.5f, 0.0f,//bot_right
	-0.5f,  0.5f, 0.0f,//top_left
	 0.5f,  0.5f, 0.0f//top_right
};

//for triangle strips
const uint32_t quad_indices_strip[] = {0, 1, 2, 3};

//for triangle
const uint32_t quad_indices[] = {0, 1, 2, 2, 1, 3};

void	setup_world_render(rend_pip_t *pipeline) {
	glGenVertexArrays(1, &pipeline->world.vao);
	glBindVertexArray(pipeline->world.vao);

	GLuint vbo;	
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices), quad_vertices, GL_STATIC_DRAW);

	GLuint ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(quad_indices), quad_indices, GL_STATIC_DRAW);
	
	glGenBuffers(1, &pipeline->world.ibo);
	glBindBuffer(GL_ARRAY_BUFFER, pipeline->world.ibo);
	glBufferData(GL_ARRAY_BUFFER, pipeline->world.faces->size * sizeof(face_data_t), pipeline->world.faces->arena, GL_DYNAMIC_DRAW);

	glVertexAttribIPointer(1, 1, GL_INT, sizeof(face_data_t), (void*)offsetof(face_data_t, face_data));
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, 1); // Update once per instance at pos idx 1
	
	glVertexAttribIPointer(2, 1, GL_INT, sizeof(face_data_t), (void*)offsetof(face_data_t, block_id));
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, 1); // Update once per instance at pos idx 2

	glBindVertexArray(0);

	//ssbo

	glGenBuffers(1, &pipeline->world.ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, pipeline->world.ssbo);
	int *buffer = malloc(sizeof(int) * 3 * pipeline->world.rqueue->size);
	for (int i = 0; i < pipeline->world.rqueue->size; i++) {
		chunk_render_t *chunk = ((chunk_render_t *)pipeline->world.rqueue);
		buffer[i * 3] = chunk[i].x;
		buffer[i * 3 + 1] = chunk[i].y;
		buffer[i * 3 + 2] = chunk[i].z;
	}
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, pipeline->world.rqueue->size * 3 * sizeof(int), buffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, pipeline->world.ssbo);
	free (buffer);

	//drawcmd

	glDrawElementsIndirectCommand cmd[pipeline->world.rqueue->size];
	for (int i = 0; i < pipeline->world.rqueue->size; i++) {
		chunk_render_t * chunk = &((chunk_render_t *)pipeline->world.rqueue->arena)[i];
		cmd[i].count = 6;
		cmd[i].instanceCount = chunk->face_count;
		cmd[i].firstIndex = 0;
		cmd[i].baseVertex = 0;
		cmd[i].baseInstance = chunk->face_offset;
	}

	glGenBuffers(1, &pipeline->indirect_buffer);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, pipeline->indirect_buffer);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(cmd), cmd, GL_DYNAMIC_DRAW);
}

void	reload_world_render(rend_pip_t *pipeline) {
	glBindVertexArray(pipeline->world.vao);

	glBindBuffer(GL_ARRAY_BUFFER, pipeline->world.ibo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, pipeline->world.faces->size * sizeof(face_data_t),  (face_data_t *)pipeline->world.faces->arena);

	glBindVertexArray(0);
	
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, pipeline->world.ssbo);
	int *buffer = malloc(sizeof(int) * 3 * pipeline->world.rqueue->size);
	for (int i = 0; i < pipeline->world.rqueue->size; i++) {
		chunk_render_t *chunk = ((chunk_render_t *)pipeline->world.rqueue);
		buffer[i * 3] = chunk[i].x;
		buffer[i * 3 + 1] = chunk[i].y;
		buffer[i * 3 + 2] = chunk[i].z;
	}
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, pipeline->world.rqueue->size * 3 * sizeof(int), buffer);
	free (buffer);

	glDrawElementsIndirectCommand cmd[pipeline->world.rqueue->size];
	for (int i = 0; i < pipeline->world.rqueue->size; i++) {
		chunk_render_t * chunk = &((chunk_render_t *)pipeline->world.rqueue->arena)[i];
		cmd[i].count = 6;
		cmd[i].instanceCount = chunk->face_count;
		cmd[i].firstIndex = 0;
		cmd[i].baseVertex = 0;
		cmd[i].baseInstance = chunk->face_offset;
	}

	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, pipeline->indirect_buffer);
	glBufferSubData(GL_DRAW_INDIRECT_BUFFER, 0, sizeof(cmd), cmd);
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
	// rlEnableBackfaceCulling();
	return (buffer);
}

void	voxel_render(engine_t *engine, world_t *world) {
	float camera_pos[3] = {engine->camera.position.x, engine->camera.position.y, engine->camera.position.z};
	int screen_height = GetScreenHeight(), screen_width = GetScreenWidth();
/*
	glBindFramebuffer(GL_FRAMEBUFFER, engine->gbuffer.framebuffer);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(engine->shader[shader_gbuffer].id);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
*/
	if (IsKeyPressed(KEY_F3)) {
		printf("reloading world render!\n");
		reload_voxel_world(world, engine);
		printf("reloaded world render\n");
		printf("%i\n", engine->render.world.rqueue->size);
	}

	BeginDrawing();
	ClearBackground(BLACK);
	BeginMode3D(engine->camera);
		
		//opaque material render
		glUseProgram(engine->shader[shader_voxel_solid].id);
		rlSetUniformMatrix(glGetUniformLocation(engine->shader[shader_voxel_solid].id, "MVP"),
			MatrixMultiply(MatrixMultiply(rlGetMatrixProjection(), rlGetMatrixModelview()),MatrixIdentity()));

		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, engine->render.world.ssbo);
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, engine->render.indirect_buffer);
		glBindVertexArray(engine->render.world.vao);
		
		glMultiDrawElementsIndirect(GL_TRIANGLES, GL_UNSIGNED_INT, 0, engine->render.world.rqueue->size, 0);

		glBindVertexArray(0);

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
