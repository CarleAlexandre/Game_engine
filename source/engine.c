#include <prototype.h>

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

	engine.shader[shader_voxel_solid].locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(engine.shader[shader_deffered], "viewPosition");

	engine.gbuffer = loadGbuffer(width, height, engine.shader[shader_deffered]);
	engine.debug = true;

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
