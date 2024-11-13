#include <engine.hpp>

// Deferred mode passes
typedef enum {
   DEFERRED_POSITION,
   DEFERRED_NORMAL,
   DEFERRED_ALBEDO,
   DEFERRED_SHADING
} DeferredMode;

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
	BeginDrawing();
		rlEnableFramebuffer(engine.gbuffer.framebuffer);
		rlClearScreenBuffers();
		rlDisableColorBlend();
		BeginMode3D(engine.camera);
			rlEnableShader(engine.gbuffer_shader.id);

			rlDisableShader();
		EndMode3D();
		rlEnableColorBlend();
		rlDisableFramebuffer();
		rlClearScreenBuffers();
	EndDrawing();

	BeginTextureMode(engine.fbo);
		ClearBackground(BLACK);
		BeginMode3D(engine.camera);
			BeginShaderMode(engine.light);
				DrawModel(level.terrain.model, level.terrain.pos, level.terrain.scale, WHITE);
				for (auto span : level.objs) {
					DrawCube(span.pos, span.scale, span.scale, span.scale, BLUE);
				}
			EndShaderMode();
		EndMode3D();
	EndTextureMode();

	BeginDrawing();
		ClearBackground(BLACK);
		BeginShaderMode(engine.posprocess);
			DrawTextureRec(engine.fbo.texture, {0, 0, (float)GetScreenWidth(), (float)-GetScreenHeight()}, {0, 0}, WHITE);
		EndShaderMode();
		render_ui();
	EndDrawing();
}
