#include <prototype.h>

void	setup_world_vao(world_t *world) {
	glGenVertexArrays(1, &world->vao);
	glBindVertexArray(world->vao);

	glGenBuffers(1, &world->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, world->vbo);
	glBufferData(GL_ARRAY_BUFFER, world->mesh.data_count * sizeof(int), world->mesh.data, GL_DYNAMIC_DRAW);
	// glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &world->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, world->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, world->mesh.index_count * sizeof(unsigned int), world->mesh.indices, GL_DYNAMIC_DRAW);
	// glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
	glVertexAttribIPointer(0, 1, GL_INT, sizeof(int), (void*)0); // Packed data is an integer
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(0);
	
}

void	setup_world_ssbo(world_t *world) {	
	glGenBuffers(1, &world->ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, world->ssbo);
	int *buffer = malloc(sizeof(int) * 3 * 27);
	for (int i = 0; i < 27; i++) {
		buffer[i * 3] = world->render_chunk[i]->x;
		buffer[i * 3 + 1] = world->render_chunk[i]->z;
		buffer[i * 3 + 2] = world->render_chunk[i]->y;
	}
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, 27 * 3 * sizeof(int), buffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, world->ssbo);
	free (buffer);
}

void	setup_indirect_buffer(rend_pip_t *render, world_t *world) {
	indirect_cmd_t cmd[27];
	for (int i = 0; i < 27; i++) {
		cmd[i].count = world->render_chunk[i]->index_count;
		cmd[i].instanceCount = 0;
		cmd[i].firstIndex = i * world->render_chunk[i]->index_offset;
		cmd[i].baseVertex = i * world->render_chunk[i]->vertex_offset;
		cmd[i].baseInstance = i;
	}

	glGenBuffers(1, &render->indirect_buffer);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, render->indirect_buffer);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(cmd), cmd, GL_DYNAMIC_DRAW);
}

void	reload_world_vao(world_t *world) {
	glBindVertexArray(world->vao);

	glBindBuffer(GL_ARRAY_BUFFER, world->vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, world->mesh.data_count * sizeof(uint64_t), world->mesh.data);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, world->ebo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, world->mesh.index_count * sizeof(unsigned int),  world->mesh.indices);

	glBindVertexArray(0);
}

void	reload_world_ssbo(world_t *world) {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, world->ssbo);
	int *buffer = malloc(sizeof(int) * 3 * world->chunk_count);
	for (int i = 0; i < 27; i++) {
		buffer[i * 3] = world->render_chunk[i]->x;
		buffer[i * 3 + 1] = world->render_chunk[i]->z;
		buffer[i * 3 + 2] = world->render_chunk[i]->y;
	}
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, 27 * 3 * sizeof(int), buffer);
	free (buffer);
}

void	reload_indirect_buffer(rend_pip_t *render, world_t *world, int chunk) {
	indirect_cmd_t cmd[chunk];
	for (int i = 0; i < chunk; i++) {
		cmd[i].count = world->render_chunk[i]->index_count;
		cmd[i].instanceCount = 0;
		cmd[i].firstIndex = i * world->render_chunk[i]->index_offset;
		cmd[i].baseVertex = i * world->render_chunk[i]->vertex_offset;
		cmd[i].baseInstance = i;
	}

	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, render->indirect_buffer);
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
	rlEnableBackfaceCulling();
	return (buffer);
}

void	voxel_render(engine_t *engine, world_t *world) {
	float camera_pos[3] = {engine->camera.position.x, engine->camera.position.y, engine->camera.position.z};
	// SetShaderValue(engine->shader[shader_voxel_solid], engine->shader[shader_voxel_solid].locs[SHADER_LOC_VECTOR_VIEW], camera_pos, SHADER_UNIFORM_VEC3);
	int screen_height = GetScreenHeight(), screen_width = GetScreenWidth();
/*
	glBindFramebuffer(GL_FRAMEBUFFER, engine->gbuffer.framebuffer);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(engine->shader[shader_gbuffer].id);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

*/
	BeginDrawing();
	ClearBackground(BLACK);
	BeginMode3D(engine->camera);

		// //opaque material render
		// glUseProgram(engine->shader[shader_voxel_solid].id);
		// rlSetUniformMatrix(glGetUniformLocation(engine->shader[shader_voxel_solid].id, "MVP"),
		// 	MatrixMultiply(MatrixMultiply(rlGetMatrixModelview(), rlGetMatrixProjection()),MatrixIdentity()));
		// glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, world->ssbo);
		// glBindBuffer(GL_DRAW_INDIRECT_BUFFER, engine->render.indirect_buffer);
		// glBindVertexArray(world->vao);
		// glMultiDrawElementsIndirect(GL_TRIANGLE_STRIP, GL_UNSIGNED_INT, 0, world->chunk_count, 0);
		// glBindVertexArray(0);

		// //transparent material render
		// glUseProgram(engine->shader[shader_voxel_solid].id);
		// rlSetUniformMatrix(glGetUniformLocation(engine->shader[shader_voxel_solid].id, "MVP"),
		// 	MatrixMultiply(MatrixMultiply(rlGetMatrixModelview(), rlGetMatrixProjection()),MatrixIdentity()));
		// glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, world->ssbo);
		// glBindBuffer(GL_DRAW_INDIRECT_BUFFER, engine->render.indirect_buffer);
		// glBindVertexArray(world->vao);
		// glMultiDrawElementsIndirect(GL_TRIANGLE_STRIP, GL_UNSIGNED_INT, 0, world->chunk_count, 0);
		// glBindVertexArray(0);

		//debug render
		DrawBoundingBox(engine->player.bound, RED);
		DrawLine3D((Vector3){-100, 0, 0}, (Vector3){100, 0, 0}, RED);
		DrawLine3D((Vector3){0, -100, 0}, (Vector3){0, 100, 0}, GREEN);
		DrawLine3D((Vector3){0, 0, -100}, (Vector3){0, 0, 100}, BLUE);
	EndMode3D();
	draw_ui(engine->player);
	EndDrawing();
}
