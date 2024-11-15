#include <engine.hpp>

#define RL_READ_FRAMEBUFFER                     0x8CA8      // GL_READ_FRAMEBUFFER
#define RL_DRAW_FRAMEBUFFER                     0x8CA9      // GL_DRAW_FRAMEBUFFER

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
	int screen_height = GetScreenHeight(), screen_width = GetScreenWidth() ;
	BeginDrawing();
	ClearBackground(RAYWHITE);
	rlEnableFramebuffer(engine.gbuffer.framebuffer);
	rlClearScreenBuffers();
	rlDisableColorBlend();
	BeginMode3D(engine.camera);
	rlEnableShader(engine.gbuffer_shader.id);
		DrawModel(engine.cube, {10, 1, 0}, 1, BLUE);
	rlDisableShader();
	EndMode3D();
	rlEnableColorBlend();
	rlDisableFramebuffer();
	rlClearScreenBuffers();

	switch(engine.mode) {
		case (DEFERRED_SHADING): {
			BeginMode3D(engine.camera);
			rlDisableColorBlend();
			rlEnableShader(engine.deffered_shader.id);
			// Activate our g-buffer textures
			rlActiveTextureSlot(0);
			rlEnableTexture(engine.gbuffer.positionTexture);
			rlActiveTextureSlot(1);
			rlEnableTexture(engine.gbuffer.normalTexture);
			rlActiveTextureSlot(2);
			rlEnableTexture(engine.gbuffer.albedoSpecTexture);
			rlLoadDrawQuad();
			rlDisableShader();
			rlEnableColorBlend();
			EndMode3D();
			glBindFramebuffer(RL_READ_FRAMEBUFFER, engine.gbuffer.framebuffer);
			glBindFramebuffer(RL_DRAW_FRAMEBUFFER, 0);
			rlBlitFramebuffer(0, 0, screen_width, screen_height, 0, 0, screen_width, screen_height, 0x00000100);    // GL_DEPTH_BUFFER_BIT
			rlDisableFramebuffer();
			// forward rendering
			BeginMode3D(engine.camera);
			rlEnableShader(rlGetShaderIdDefault());
			rlDisableShader();
			EndMode3D();
			
			DrawText("FINAL RESULT", 10, screen_height - 30, 20, DARKGREEN);
			break;
		}
		case (DEFERRED_POSITION): {
			DrawTextureRec((Texture2D){
			.id = engine.gbuffer.positionTexture,
			.width = screen_width,
			.height = screen_height,
			}, (Rectangle) { 0, 0, (float)screen_width, (float)-screen_height }, Vector2Zero(), RAYWHITE);

			DrawText("POSITION TEXTURE", 10, screen_height - 30, 20, DARKGREEN);
			break;
		}
		case (DEFERRED_NORMAL): {
			DrawTextureRec((Texture2D){
			.id = engine.gbuffer.normalTexture,
			.width = screen_width,
			.height = screen_height,
			}, (Rectangle) { 0, 0, (float)screen_width, (float)-screen_height }, Vector2Zero(), RAYWHITE);

			DrawText("NORMAL TEXTURE", 10, screen_height - 30, 20, DARKGREEN);
			break;
		}
		case (DEFERRED_ALBEDO): {
			DrawTextureRec((Texture2D){
			.id = engine.gbuffer.albedoSpecTexture,
			.width = screen_width,
			.height = screen_height,
			}, (Rectangle) { 0, 0, (float)screen_width, (float)-screen_height }, Vector2Zero(), RAYWHITE);

			DrawText("ALBEDO TEXTURE", 10, screen_height - 30, 20, DARKGREEN);
			break;
		}
		default:break;
	};
	EndDrawing();
}
