#include <raylib.h>
#include <engine.hpp>

void update_light(std::vector<light_t> &lights, Shader light_shader) {
	for (auto light: lights) {
		SetShaderValue(light_shader, light.pos_loc, &light.pos, SHADER_UNIFORM_VEC2);
		SetShaderValue(light_shader, light.type_loc, &light.type, SHADER_UNIFORM_INT);
		SetShaderValue(light_shader, light.intensity_loc, &light.intensity, SHADER_UNIFORM_INT);
	}
}

void create_light(std::vector<light_t> &lights, Vector3 pos, int type, int intensity, Shader light_shader) {
	light_t light = {0};

	light.intensity = intensity;
	light.pos = pos;
	light.type = type;

	light.pos_loc = GetShaderLocation(light_shader, "light[%i].pos");
	light.type_loc = GetShaderLocation(light_shader, "light[%i].type");
	light.intensity_loc = GetShaderLocation(light_shader, "light[%i].intensity");

	lights.push_back(light);
	update_light(lights, light_shader);
}

void render(level_t level, RenderTexture2D fbo, Camera3D camera, Shader posprocess_shader, Shader light_shader) {
	BeginTextureMode(fbo);
		ClearBackground(BLACK);
		BeginMode3D(camera);
			DrawModel(level.terrain.model, level.terrain.pos, level.terrain.scale, WHITE);
		EndMode2D();
	EndTextureMode();

	BeginDrawing();
		ClearBackground(BLACK);
		BeginShaderMode(posprocess_shader);
			DrawTextureRec(fbo.texture, {0, 0, (float)GetScreenWidth(), (float)-GetScreenHeight()}, {0, 0}, WHITE);
		EndShaderMode();
	EndDrawing();
}
