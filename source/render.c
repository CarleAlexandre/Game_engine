#include <prototype.h>

int64_t *buffer;

void	voxel_render(engine_t *engine, world_t *world) {
	float camera_pos[3] = {engine->camera.position.x, engine->camera.position.y, engine->camera.position.z};
	SetShaderValue(engine->shader[shader_voxel_solid], engine->shader[shader_voxel_solid].locs[SHADER_LOC_VECTOR_VIEW], camera_pos, SHADER_UNIFORM_VEC3);
	int screen_height = GetScreenHeight(), screen_width = GetScreenWidth();
/*
	glBindFramebuffer(GL_FRAMEBUFFER, engine->gbuffer.framebuffer);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUseProgram(engine->shader[shader_gbuffer].id);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

*/

	BeginDrawing();
	ClearBackground(BLACK);
	BeginMode3D(engine->camera);
		glUseProgram(engine->shader[shader_voxel_solid].id);

		rlSetUniformMatrix(glGetUniformLocation(engine->shader[shader_voxel_solid].id, "MVP"), 
			MatrixMultiply(MatrixMultiply(rlGetMatrixModelview(), rlGetMatrixProjection()),MatrixIdentity()));

		glBindVertexArray(world->vao);
		
		glMultiDrawArraysIndirect(GL_TRIANGLE_STRIP, 0x00, 4, sizeof(int64_t));

		glBindVertexArray(0);

		DrawBoundingBox(engine->player.bound, RED);
		DrawLine3D((Vector3){-100, 0, 0}, (Vector3){100, 0, 0}, RED);
		DrawLine3D((Vector3){0, -100, 0}, (Vector3){0, 100, 0}, GREEN);
		DrawLine3D((Vector3){0, 0, -100}, (Vector3){0, 0, 100}, BLUE);
	EndMode3D();
	draw_ui(engine->player);
	EndDrawing();
}

// void	render(engine_t *engine, chunk_t *world[5][5]) {
// 	float camera_pos[3] = {engine->camera.position.x, engine->camera.position.y, engine->camera.position.z};
// 	SetShaderValue(engine->deffered_shader, engine->deffered_shader.locs[SHADER_LOC_VECTOR_VIEW], camera_pos, SHADER_UNIFORM_VEC3);

// 	int screen_height = GetScreenHeight(), screen_width = GetScreenWidth();

// 	BeginDrawing();
// 	ClearBackground(BLACK);
// 	rlEnableFramebuffer(engine->gbuffer.framebuffer);
// 	rlClearScreenBuffers();
// 	rlDisableColorBlend();
// 	BeginMode3D(engine->camera);
// 	rlEnableShader(engine->gbuffer_shader.id);

// 	//draw model here

// 	rlDisableShader();
// 	EndMode3D();
// 	rlEnableColorBlend();
// 	rlDisableFramebuffer();
// 	rlClearScreenBuffers();

// 	switch(engine->mode) {
// 		case (DEFERRED_SHADING): {
// 			BeginMode3D(engine->camera);
// 			rlDisableColorBlend();
// 			rlEnableShader(engine->deffered_shader.id);
// 			rlActiveTextureSlot(0);
// 			rlEnableTexture(engine->gbuffer.positionTexture);
// 			rlActiveTextureSlot(1);
// 			rlEnableTexture(engine->gbuffer.normalTexture);
// 			rlActiveTextureSlot(2);
// 			rlEnableTexture(engine->gbuffer.albedoSpecTexture);
// 			rlActiveTextureSlot(3);
// 			rlEnableTexture(engine->gbuffer.zTexture);
// 			rlLoadDrawQuad();
// 			rlDisableShader();
// 			rlEnableColorBlend();
// 			EndMode3D();

// 			glBindFramebuffer(RL_READ_FRAMEBUFFER, engine->gbuffer.framebuffer);
// 			glBindFramebuffer(RL_DRAW_FRAMEBUFFER, 0);
// 			rlBlitFramebuffer(0, 0, screen_width, screen_height, 0, 0, screen_width, screen_height, GL_DEPTH_BUFFER_BIT);
// 			rlDisableFramebuffer();

// 			BeginMode3D(engine->camera);
// 			rlEnableShader(rlGetShaderIdDefault());

// 			DrawBoundingBox(engine->player.bound, RED);

// 			rlDisableShader();
// 			EndMode3D();

// 			DrawText("FINAL RESULT", 10, screen_height - 30, 20, DARKGREEN);
// 			break;
// 		}
// 		case (DEFERRED_POSITION): {
// 			DrawTextureRec((Texture2D){
// 			.id = engine->gbuffer.positionTexture,
// 			.width = screen_width,
// 			.height = screen_height,
// 			}, (Rectangle) { 0, 0, (float)screen_width, (float)-screen_height }, Vector2Zero(), WHITE);

// 			DrawText("POSITION TEXTURE", 10, screen_height - 30, 20, DARKGREEN);
// 			break;
// 		}
// 		case (DEFERRED_NORMAL): {
// 			DrawTextureRec((Texture2D){
// 			.id = engine->gbuffer.normalTexture,
// 			.width = screen_width,
// 			.height = screen_height,
// 			}, (Rectangle) { 0, 0, (float)screen_width, (float)-screen_height }, Vector2Zero(), WHITE);

// 			DrawText("NORMAL TEXTURE", 10, screen_height - 30, 20, DARKGREEN);
// 			break;
// 		}
// 		case (DEFERRED_ALBEDO): {
// 			DrawTextureRec((Texture2D){
// 			.id = engine->gbuffer.albedoSpecTexture,
// 			.width = screen_width,
// 			.height = screen_height,
// 			}, (Rectangle) { 0, 0, (float)screen_width, (float)-screen_height }, Vector2Zero(), WHITE);

// 			DrawText("ALBEDO TEXTURE", 10, screen_height - 30, 20, DARKGREEN);
// 			break;
// 		}
// 		case (DEFERRED_Z): {
// 			DrawTextureRec((Texture2D){
// 			.id = engine->gbuffer.zTexture,
// 			.width = screen_width,
// 			.height = screen_height,
// 			}, (Rectangle) { 0, 0, (float)screen_width, (float)-screen_height }, Vector2Zero(), WHITE);

// 			DrawText("Z TEXTURE", 10, screen_height - 30, 20, DARKGREEN);
// 			break;
// 		}
// 		default:break;
// 	};
// 	draw_ui(engine->player);
// 	EndDrawing();
// }
