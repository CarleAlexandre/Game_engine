#include <prototype.h>

engine_t	init_engine(void) {
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

	Mesh cube = GenMeshCube(1, 1, 1);

	engine.skybox = LoadModelFromMesh(cube);

	engine.skybox.materials[0].shader = LoadShader("shader/skybox.vs", "shader/skybox.fs");

	SetShaderValue(engine.skybox.materials[0].shader, GetShaderLocation(engine.skybox.materials[0].shader, "environmentMap"), (int[1]){ MATERIAL_MAP_CUBEMAP }, SHADER_UNIFORM_INT);
	SetShaderValue(engine.skybox.materials[0].shader, GetShaderLocation(engine.skybox.materials[0].shader, "doGamma"), (int[1]){ 0 }, SHADER_UNIFORM_INT);
	SetShaderValue(engine.skybox.materials[0].shader, GetShaderLocation(engine.skybox.materials[0].shader, "vflipped"), (int[1]){ 0 }, SHADER_UNIFORM_INT);

	Image img = LoadImage("include/ext/skybox.png");
	engine.skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(img, CUBEMAP_LAYOUT_AUTO_DETECT);
	UnloadImage(img);

	return (engine);
}

void	reload_shader(engine_t *engine) {
	for (int i = 0; i < 8; i++) {
		if (engine->shader[i].id) {
			UnloadShader(engine->shader[i]);
		}
	}

	engine->shader[shader_sobel] = LoadShader(0, "shader/sobel.fs");
	engine->shader[shader_deffered] = LoadShader("shader/defered.vs", "shader/defered.fs");
	engine->shader[shader_gbuffer] = LoadShader("shader/gbuffer.vs", "shader/gbuffer.fs");
	engine->shader[shader_voxel_solid] = LoadShader("shader/vox_solid.vs", "shader/vox_solid.fs");

	engine->shader[shader_deffered].locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(engine->shader[shader_deffered], "viewPosition");
	engine->shader[shader_voxel_solid].locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(engine->shader[shader_deffered], "viewPosition");
}

void	close_engine(engine_t *engine) {
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
