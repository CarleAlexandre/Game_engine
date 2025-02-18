#include <prototype.h>

void	extract_frustum_planes(Matrix view_proj, plane_t *frustum) {
	// Left plane
	frustum[0].normal.x = view_proj.m0 + view_proj.m3;
	frustum[0].normal.y = view_proj.m4 + view_proj.m7;
	frustum[0].normal.z = view_proj.m8 + view_proj.m11;
	frustum[0].distance = view_proj.m12 + view_proj.m15;
    
	// Right plane
	frustum[1].normal.x = -view_proj.m0 + view_proj.m3;
	frustum[1].normal.y = -view_proj.m4 + view_proj.m7;
	frustum[1].normal.z = -view_proj.m8 + view_proj.m11;
	frustum[1].distance = -view_proj.m12 + view_proj.m15;
    
	// Bottom plane
	frustum[2].normal.x = view_proj.m1 + view_proj.m3;
	frustum[2].normal.y = view_proj.m5 + view_proj.m7;
	frustum[2].normal.z = view_proj.m9 + view_proj.m11;
	frustum[2].distance = view_proj.m13 + view_proj.m15;
    
	// Top plane
	frustum[3].normal.x = -view_proj.m1 + view_proj.m3;
	frustum[3].normal.y = -view_proj.m5 + view_proj.m7;
	frustum[3].normal.z = -view_proj.m9 + view_proj.m11;
	frustum[3].distance = -view_proj.m13 + view_proj.m15;
    
	// Near plane
	frustum[4].normal.x = view_proj.m2 + view_proj.m3;
	frustum[4].normal.y = view_proj.m6 + view_proj.m7;
	frustum[4].normal.z = view_proj.m10 + view_proj.m11;
	frustum[4].distance = view_proj.m14 + view_proj.m15;
    
	// Far plane
	frustum[5].normal.x = -view_proj.m2 + view_proj.m3;
	frustum[5].normal.y = -view_proj.m6 + view_proj.m7;
	frustum[5].normal.z = -view_proj.m10 + view_proj.m11;
	frustum[5].distance = -view_proj.m14 + view_proj.m15;

	for (int i = 0; i < 6; i++) {
		float length = Vector3Length(frustum[i].normal);
		frustum[i].normal = Vector3Normalize(frustum[i].normal);
		frustum[i].distance /= length;
	}
}

void	setup_world_vao(world_render_t *world) {
	glGenVertexArrays(1, &world->vao);
	glBindVertexArray(world->vao);

	glGenBuffers(1, &world->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, world->vbo);
	glBufferData(GL_ARRAY_BUFFER, world->mesh.face_count * sizeof(face_data_t), world->mesh.faces, GL_DYNAMIC_DRAW);
	
	glVertexAttribIPointer(0, 1, GL_INT, sizeof(int), (void*)0); // Packed data is an integer
	glEnableVertexAttribArray(0);
	
	glBindVertexArray(0);
	
}

void	setup_world_ssbo(world_render_t *world) {
	glGenBuffers(1, &world->ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, world->ssbo);
	int *buffer = malloc(sizeof(int) * 3 * world->rqueue->size);
	for (int i = 0; i < world->rqueue->size; i++) {
		buffer[i * 3] = ((chunk_render_t *)world->rqueue)[i].x;
		buffer[i * 3 + 1] = ((chunk_render_t *)world->rqueue)[i].z;
		buffer[i * 3 + 2] = ((chunk_render_t *)world->rqueue)[i].y;
	}
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, world->rqueue->size * 3 * sizeof(int), buffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, world->ssbo);
	free (buffer);
}

void	setup_indirect_buffer(rend_pip_t *render) {
	indirect_cmd_t cmd[render->world.rqueue->size];
	for (int i = 0; i < render->world.rqueue->size; i++) {
		cmd[i].count = ((chunk_render_t *)render->world.rqueue)[i].face_count;
		cmd[i].instanceCount = 0;
		cmd[i].first = i * ((chunk_render_t *)render->world.rqueue)[i].face_offset;
		cmd[i].baseInstance = i;
	}

	glGenBuffers(1, &render->indirect_buffer);
	glBindBuffer(GL_DRAW_INDIRECT_BUFFER, render->indirect_buffer);
	glBufferData(GL_DRAW_INDIRECT_BUFFER, sizeof(cmd), cmd, GL_DYNAMIC_DRAW);
}

void	reload_world_vao(world_render_t *world) {
	glBindVertexArray(world->vao);

	glBindBuffer(GL_ARRAY_BUFFER, world->vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, world->mesh.face_count * sizeof(face_data_t), world->mesh.faces);

	glBindVertexArray(0);
}

void	reload_world_ssbo(world_render_t *world) {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, world->ssbo);
	int *buffer = malloc(sizeof(int) * 3 * world->rqueue->size);
	for (int i = 0; i < world->rqueue->size; i++) {
		buffer[i * 3] = ((chunk_render_t *)world->rqueue)[i].x;
		buffer[i * 3 + 1] = ((chunk_render_t *)world->rqueue)[i].z;
		buffer[i * 3 + 2] = ((chunk_render_t *)world->rqueue)[i].y;
	}
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, world->rqueue->size * 3 * sizeof(int), buffer);
	free (buffer);
}

void	reload_indirect_buffer(rend_pip_t *render) {
	indirect_cmd_t cmd[render->world.rqueue->size];
	for (int i = 0; i < render->world.rqueue->size; i++) {
		cmd[i].count = ((chunk_render_t *)render->world.rqueue)[i].face_count;
		cmd[i].instanceCount = 1;
		cmd[i].first = i * ((chunk_render_t *)render->world.rqueue)[i].face_offset;
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
	int screen_height = GetScreenHeight(), screen_width = GetScreenWidth();
/*
	glBindFramebuffer(GL_FRAMEBUFFER, engine->gbuffer.framebuffer);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(engine->shader[shader_gbuffer].id);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
*/
	if (IsKeyPressed(KEY_F3)) {
		reload_voxel_world(world, engine);
	}

	BeginDrawing();
	ClearBackground(BLACK);
	BeginMode3D(engine->camera);
		
		//opaque material render
		glUseProgram(engine->shader[shader_voxel_solid].id);
		rlSetUniformMatrix(glGetUniformLocation(engine->shader[shader_voxel_solid].id, "MVP"),
			MatrixMultiply(MatrixMultiply(rlGetMatrixModelview(), rlGetMatrixProjection()),MatrixIdentity()));
		glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, engine->render.world.ssbo);
		glBindBuffer(GL_DRAW_INDIRECT_BUFFER, engine->render.indirect_buffer);
		glBindVertexArray(engine->render.world.vao);
		
		glMultiDrawArraysIndirect(GL_TRIANGLE_STRIP, 0, engine->render.world.rqueue->size, 0);
		glBindVertexArray(0);

		// //transparent material render
		// glUseProgram(engine->shader[shader_voxel_solid].id);
		// rlSetUniformMatrix(glGetUniformLocation(engine->shader[shader_voxel_solid].id, "MVP"),
		// 	MatrixMultiply(MatrixMultiply(rlGetMatrixModelview(), rlGetMatrixProjection()),MatrixIdentity()));
		// glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, world->ssbo);
		// glBindBuffer(GL_DRAW_INDIRECT_BUFFER, engine->render.indirect_buffer);
		// glBindVertexArray(world->vao);
		// glMultiDrawElementsIndirect(GL_TRIANGLE_STRIP, GL_UNSIGNED_INT, 0, world->chunk_count, 0);
		// glBindVertexArray(0);

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
