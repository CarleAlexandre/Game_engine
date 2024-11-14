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
	BeginDrawing();
	ClearBackground(RAYWHITE);
	rlEnableFramebuffer(engine.gbuffer.framebuffer);
	rlClearScreenBuffers();
	rlDisableColorBlend();
	BeginMode3D(engine.camera);
	rlEnableShader(engine.gbuffer_shader.id);
	DrawCube({0}, 10, 10, 10, BLUE);
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

			DrawCube({10, 1, 0}, 10, 10, 10, BLUE);
			EndMode3D();
			//rlBindFramebuffer(RL_READ_FRAMEBUFFER, engine.gbuffer.framebuffer);
			//rlBindFramebuffer(RL_DRAW_FRAMEBUFFER, 0);
			//cannot get above function
			glBindTexture(RL_READ_FRAMEBUFFER, engine.gbuffer.framebuffer);
			glBindTexture(RL_DRAW_FRAMEBUFFER, 0);
			rlBlitFramebuffer(0, 0, GetScreenWidth(), GetScreenHeight(), 0, 0, GetScreenWidth(), GetScreenHeight(), 0x00000100);    // GL_DEPTH_BUFFER_BIT
			rlDisableFramebuffer();
			// forward rendering
			BeginMode3D(engine.camera);
			rlEnableShader(rlGetShaderIdDefault());
			rlDisableShader();
			EndMode3D();
			
			DrawText("FINAL RESULT", 10, GetScreenHeight() - 30, 20, DARKGREEN);
			break;
		}
		case (DEFERRED_POSITION): {
			break;
		}
		case (DEFERRED_NORMAL): {
			break;
		}
		case (DEFERRED_ALBEDO): {
			break;
		}
		default:break;
	};
	EndDrawing();
	// BeginTextureMode(engine.fbo);
	// 	ClearBackground(BLACK);
	// 	BeginMode3D(engine.camera);
	// 		BeginShaderMode(engine.light);
	// 			//DrawModel(level.terrain.model, level.terrain.pos, level.terrain.scale, WHITE);
	// 			for (auto span : level.objs) {
	// 				DrawCube(span.pos, span.scale, span.scale, span.scale, BLUE);
	// 			}
	// 			DrawCube({0}, 10, 10, 10, BLUE);
	// 		EndShaderMode();
	// 	EndMode3D();
	// EndTextureMode();

	// BeginDrawing();
	// 	ClearBackground(BLACK);
	// 	BeginShaderMode(engine.posprocess);
	// 		DrawTextureRec(engine.fbo.texture, {0, 0, (float)GetScreenWidth(), (float)-GetScreenHeight()}, {0, 0}, WHITE);
	// 	EndShaderMode();
	// 	//render_ui();
	// EndDrawing();
}
