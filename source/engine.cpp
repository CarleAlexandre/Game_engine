#include <prototype.hpp>
#include <iostream>

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
	return (buffer);
}

engine_t init_engine(void) {
	int width = GetScreenWidth(), height= GetScreenHeight();
	engine_t engine = {0};
	Camera3D camera = {0};
	camera.fovy = 90;
	camera.position = {0, 1, 0};
	camera.projection = CAMERA_PERSPECTIVE;
	camera.target = {1, 1, 0};
	camera.up = {0, 1, 0};
	engine.camera = camera;

	engine.cube = LoadModelFromMesh(GenMeshCube(10, 10, 10));
	engine.Sphere = LoadModelFromMesh(GenMeshSphere(5, 10, 10));

	engine.posprocess = LoadShader(0, "shader/postprocess.fs");
	engine.deffered_shader = LoadShader("shader/defered.vs", "shader/defered.fs");
	engine.gbuffer_shader = LoadShader("shader/gbuffer.vs", "shader/gbuffer.fs");
	engine.deffered_shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(engine.deffered_shader, "viewPosition");

	engine.gbuffer = loadGbuffer(width, height, engine.deffered_shader) ;

	engine.mode = DEFERRED_SHADING;

	engine.cube.materials[0].shader = engine.gbuffer_shader;
	engine.Sphere.materials[0].shader = engine.gbuffer_shader;

	engine.lights[0] = CreateLight(LIGHT_POINT, (Vector3){ -0, 40, -100 }, Vector3Zero(), YELLOW, engine.deffered_shader);
	engine.lights[1] = CreateLight(LIGHT_POINT, (Vector3){ 0, 40, 100 }, Vector3Zero(), RED, engine.deffered_shader);
	engine.lights[2] = CreateLight(LIGHT_POINT, (Vector3){ -100, 40, 0 }, Vector3Zero(), GREEN, engine.deffered_shader);
	engine.lights[3] = CreateLight(LIGHT_POINT, (Vector3){ 100, 40, -0 }, Vector3Zero(), BLUE, engine.deffered_shader);

	SetTargetFPS(60);
	return (engine);
}

void close_engine(engine_t &engine) {
	UnloadShader(engine.deffered_shader);
	UnloadShader(engine.gbuffer_shader);
	UnloadShader(engine.posprocess);

	UnloadRenderTexture(engine.fbo);
	rlUnloadFramebuffer(engine.gbuffer.framebuffer);
	UnloadModel(engine.cube);
	UnloadModel(engine.Sphere);
	rlUnloadTexture(engine.gbuffer.positionTexture);
	rlUnloadTexture(engine.gbuffer.normalTexture);
	rlUnloadTexture(engine.gbuffer.albedoSpecTexture);
	rlUnloadTexture(engine.gbuffer.zTexture);
	rlUnloadTexture(engine.gbuffer.depthRenderbuffer);
	CloseWindow();
}
