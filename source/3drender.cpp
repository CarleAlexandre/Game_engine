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

void render(level_t level, engine_t &engine, void (*render_ui)(void)) {
	BeginTextureMode(engine.fbo);
		ClearBackground(BLACK);
		BeginShaderMode(engine.light);
			BeginMode3D(engine.camera);
				DrawModel(level.terrain.model, level.terrain.pos, level.terrain.scale, WHITE);
				for (auto span : level.objs) {
					DrawCube(span.pos, span.scale, span.scale, span.scale, BLUE);
				}
			EndMode3D();
		EndShaderMode();
	EndTextureMode();

	BeginDrawing();
		ClearBackground(BLACK);
		BeginShaderMode(engine.posprocess);
			DrawTextureRec(engine.fbo.texture, {0, 0, (float)GetScreenWidth(), (float)-GetScreenHeight()}, {0, 0}, WHITE);
		EndShaderMode();
		render_ui();
	EndDrawing();
}
