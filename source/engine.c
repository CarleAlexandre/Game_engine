#include <engine.h>

void	setup_world_vao(world_t *world) {
	glGenVertexArrays(1, &world->vao);
	glBindVertexArray(world->vao);

	glGenBuffers(1, &world->vbo);
	glBindBuffer(GL_ARRAY_BUFFER, world->vbo);
	glBufferData(GL_ARRAY_BUFFER, world->mesh.data_count * sizeof(int), world->mesh.data, GL_DYNAMIC_DRAW);
    
	glGenBuffers(1, &world->ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, world->ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, world->mesh.index_count * sizeof(unsigned int), world->mesh.indices, GL_DYNAMIC_DRAW);

	glGenBuffers(1, &world->ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, world->ssbo);
	glBufferData(GL_SHADER_STORAGE_BUFFER, world->mesh.chunk_count * sizeof(unsigned int *), world->mesh.chunk_pos, GL_DYNAMIC_STORAGE_BIT);

	glVertexAttribIPointer(0, 1, GL_INT, sizeof(int), (void*)0); // Packed data is an integer
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

}

void	reload_world_vao(world_t *world) {
	glBindVertexArray(world->vao);

	glBindBuffer(GL_ARRAY_BUFFER, world->vbo);
	glBufferSubData(GL_ARRAY_BUFFER, 0, world->mesh.data_count * sizeof(uint64_t), world->mesh.data);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, world->ebo);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, world->mesh.index_count * sizeof(unsigned int),  world->mesh.indices);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, world->ssbo);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, world->mesh.chunk_count * sizeof(unsigned int *), world->mesh.chunk_pos);

	glBindVertexArray(0);
}

static gbuffer_t loadGbuffer(int width, int height, Shader deffered_shader) {
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

engine_t init_engine(void) {
	int width = GetScreenWidth(), height= GetScreenHeight();
	engine_t engine = {0};
	Camera3D camera = {0};
	camera.fovy = 90;
	camera.position = (Vector3){0, 1, 0};
	camera.projection = CAMERA_PERSPECTIVE;
	camera.target = (Vector3){1, 1, 0};
	camera.up = (Vector3){0, 1, 0};
	engine.camera = camera;

	engine.shader[shader_sobel] = LoadShader(0, "shader/sobel.fs");
	engine.shader[shader_deffered] = LoadShader("shader/defered.vs", "shader/defered.fs");
	engine.shader[shader_gbuffer] = LoadShader("shader/gbuffer.vs", "shader/gbuffer.fs");
	engine.shader[shader_voxel_solid] = LoadShader("shader/vox_solid.vs", "shader/vox_solid.fs");
	engine.shader[shader_deffered].locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(engine.shader[shader_deffered], "viewPosition");

	engine.gbuffer = loadGbuffer(width, height, engine.shader[shader_deffered]) ;

	return (engine);
}

void close_engine(engine_t *engine) {
	for (int i = 0; i < 8; i++) {
		if (engine->shader[i].id) {
			UnloadShader(engine->shader[i]);
		}
	}
	rlUnloadFramebuffer(engine->gbuffer.framebuffer);
	rlUnloadTexture(engine->gbuffer.positionTexture);
	rlUnloadTexture(engine->gbuffer.normalTexture);
	rlUnloadTexture(engine->gbuffer.albedoSpecTexture);
	rlUnloadTexture(engine->gbuffer.zTexture);
	rlUnloadTexture(engine->gbuffer.depthRenderbuffer);
	CloseWindow();
}
