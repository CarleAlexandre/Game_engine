#include <raylib.h>

Model	haven_rl_skybox_create(void) {
	Mesh cube = GenMeshCube(1, 1, 1);
	Model skybox = LoadModelFromMesh(cube);
	skybox.materials[0].shader = LoadShader("shader/skybox.vs", "shader/skybox.fs");
	SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "environmentMap"), (int[1]){ MATERIAL_MAP_CUBEMAP }, SHADER_UNIFORM_INT);
	SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "doGamma"), (int[1]){ 0 }, SHADER_UNIFORM_INT);
	SetShaderValue(skybox.materials[0].shader, GetShaderLocation(skybox.materials[0].shader, "vflipped"), (int[1]){ 0 }, SHADER_UNIFORM_INT);
	Image img = LoadImage("include/ext/skybox.png");
	skybox.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(img, CUBEMAP_LAYOUT_AUTO_DETECT);
	UnloadImage(img);

	return (skybox);
}
