#include "game.h"

void	scene_render(const Model skybox, Camera3D *camera, const gbuffer_t gbuffer, const Shader shader[MAX_SHADER], int deferred_mode) {
	UpdateCamera(camera, CAMERA_FREE);	
	float camerapos[3] = {camera->position.x, camera->position.y, camera->position.z};
	SetShaderValue(shader[SHADER_DEFERRED], shader[SHADER_DEFERRED].locs[SHADER_LOC_VECTOR_VIEW], camerapos, SHADER_UNIFORM_VEC3);
	SetMousePosition(GetScreenWidth() * 0.5, GetScreenHeight() * 0.5);

	//update all light;

	BeginDrawing(); {
		haven_gbuffer_start_draw(gbuffer, *camera, shader[SHADER_GBUFFER]); {
			//drawDeferred scene here
		} haven_gbuffer_end_draw();
		switch (deferred_mode){
			case (DEFERRED_SHADING) : {
				haven_gbuffer_rendering(gbuffer, *camera, shader[SHADER_DEFERRED]);
				BeginMode3D(*camera); {
					rlDisableBackfaceCulling();
					rlDisableDepthMask();
					DrawModel(skybox, Vector3Zero(), 1.0f, WHITE);
					rlEnableBackfaceCulling();
					rlEnableDepthMask();	
					
					rlEnableShader(rlGetShaderIdDefault()); {
						DrawCube((Vector3){10, 10, 10}, 10, 10, 10, RED);
					} rlDisableShader();
				}EndMode3D();
				break;
			}
			case (DEFERRED_POSITION) : {
				haven_gbuffer_texture_render(gbuffer.positionTexture, gbuffer.width, gbuffer.height);
				break;
			}
			case (DEFERRED_NORMAL) : {
				haven_gbuffer_texture_render(gbuffer.normalTexture, gbuffer.width, gbuffer.height);
				break;
			}
			case (DEFERRED_ALBEDO) : {
				haven_gbuffer_texture_render(gbuffer.albedoSpecTexture, gbuffer.width, gbuffer.height);
				break;
			}
			case (DEFERRED_ZBUFFER) : {
				haven_gbuffer_texture_render(gbuffer.zTexture, gbuffer.width, gbuffer.height);
				break;
			}
			default:{
				break;
			}
		}
		DrawFPS(10, 10);
		DrawText(TextFormat("%f", GetFrameTime() * 1000), 30, 30, 10, GREEN);
		render_debug_draw();

	} EndDrawing();
}
