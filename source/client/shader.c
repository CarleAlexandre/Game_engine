#include "game.h"

Shader	*game_shader_load(void) {
	Shader *shader = malloc(sizeof(Shader) * MAX_SHADER);
	assert(shader);

	shader[SHADER_GBUFFER] = LoadShader("shader/gbuffer.vs", "shader/gbuffer.fs");
	shader[SHADER_DEFERRED] = LoadShader("shader/deferred.vs", "shader/deferred.fs");
	shader[SHADER_DEFERRED].locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader[SHADER_DEFERRED], "viewPosition");
	shader[SHADER_SKYBOX] = LoadShader("shader/skybox.vs", "shader/skybox.fs");
	shader[SHADER_VOXEL] = LoadShader("shader/vox_solid.vs", "shader/vox_solid.fs");
	shader[SHADER_SOBEL] = LoadShader(0x00, "shader/sobel.fs");
	shader[SHADER_PBR] = LoadShader("shader/pbr.vs", "shader/pbr.fs");
	return (shader);
}

void	game_shader_unload(Shader *shader) {
	for (int i = 0; i < MAX_SHADER; i++) {
		UnloadShader(shader[i]);
	}
	free (shader);
}
