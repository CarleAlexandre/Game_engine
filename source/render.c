#include <prototype.h>

static int lightsCount = 0;

light_t CreateLight(int type, Vector3 position, Vector3 target, Color color, Shader shader) {
    light_t light = { 0 };

    if (lightsCount < MAX_LIGHTS) {
        light.enabled = true;
        light.type = type;
        light.position = position;
        light.target = target;
        light.color = color;

        // NOTE: Lighting shader naming must be the provided ones
        light.enabledLoc = GetShaderLocation(shader, TextFormat("lights[%i].enabled", lightsCount));
        light.typeLoc = GetShaderLocation(shader, TextFormat("lights[%i].type", lightsCount));
        light.positionLoc = GetShaderLocation(shader, TextFormat("lights[%i].position", lightsCount));
        light.targetLoc = GetShaderLocation(shader, TextFormat("lights[%i].target", lightsCount));
        light.colorLoc = GetShaderLocation(shader, TextFormat("lights[%i].color", lightsCount));

        UpdateLightValues(shader, light);
        
        lightsCount++;
    }

    return light;
}

void UpdateLightValues(Shader shader, light_t light) {
    // Send to shader light enabled state and type
    SetShaderValue(shader, light.enabledLoc, &light.enabled, SHADER_UNIFORM_INT);
    SetShaderValue(shader, light.typeLoc, &light.type, SHADER_UNIFORM_INT);

    // Send to shader light position values
    float position[3] = { light.position.x, light.position.y, light.position.z };
    SetShaderValue(shader, light.positionLoc, position, SHADER_UNIFORM_VEC3);

    // Send to shader light target position values
    float target[3] = { light.target.x, light.target.y, light.target.z };
    SetShaderValue(shader, light.targetLoc, target, SHADER_UNIFORM_VEC3);

    // Send to shader light color values
    float color[4] = { (float)light.color.r/(float)255, (float)light.color.g/(float)255, 
                       (float)light.color.b/(float)255, (float)light.color.a/(float)255 };
    SetShaderValue(shader, light.colorLoc, color, SHADER_UNIFORM_VEC4);
}

void	voxel_render(engine_t *engine, chunk_t *world[5][5]) {
	float camera_pos[3] = {engine->camera.position.x, engine->camera.position.y, engine->camera.position.z};
	SetShaderValue(engine->vox_shader, engine->vox_shader.locs[SHADER_LOC_VECTOR_VIEW], camera_pos, SHADER_UNIFORM_VEC3);
	int screen_height = GetScreenHeight(), screen_width = GetScreenWidth();

	// clear_chunk_mesh(world[0][0]);
	// generate_chunk_mesh(world[0][0]);
	// reload_chunk_buffers(world[0][0]);

	BeginDrawing();
	ClearBackground(BLACK);
	BeginMode3D(engine->camera);
		glUseProgram(engine->vox_shader.id);
		for (int x = 0; x < 5; x++){
			for (int z = 0; z < 5; z++){
				render_vox_mesh(world[x][z]);
			}
		}
	
		for (int x = 0; x < 5; x++){
			for (int z = 0; z < 5; z++){
				render_vox_trans(world[x][z]);
			}
		}
	
	DrawLine3D(Vector3Zero(), (Vector3){100, 0, 0}, RED);
	DrawLine3D(Vector3Zero(), (Vector3){0, 100, 0}, GREEN);
	DrawLine3D(Vector3Zero(), (Vector3){0, 0, 100}, BLUE);
	EndMode3D();
	draw_ui(engine->player);
	EndDrawing();
}

void	render(engine_t *engine, chunk_t *world[5][5]) {
	float camera_pos[3] = {engine->camera.position.x, engine->camera.position.y, engine->camera.position.z};
	SetShaderValue(engine->deffered_shader, engine->deffered_shader.locs[SHADER_LOC_VECTOR_VIEW], camera_pos, SHADER_UNIFORM_VEC3);

	int screen_height = GetScreenHeight(), screen_width = GetScreenWidth();

	BeginDrawing();
	ClearBackground(BLACK);
	rlEnableFramebuffer(engine->gbuffer.framebuffer);
	rlClearScreenBuffers();
	rlDisableColorBlend();
	BeginMode3D(engine->camera);
	rlEnableShader(engine->gbuffer_shader.id);

	//draw model here

	rlDisableShader();
	EndMode3D();
	rlEnableColorBlend();
	rlDisableFramebuffer();
	rlClearScreenBuffers();

	switch(engine->mode) {
		case (DEFERRED_SHADING): {
			BeginMode3D(engine->camera);
			rlDisableColorBlend();
			rlEnableShader(engine->deffered_shader.id);
			rlActiveTextureSlot(0);
			rlEnableTexture(engine->gbuffer.positionTexture);
			rlActiveTextureSlot(1);
			rlEnableTexture(engine->gbuffer.normalTexture);
			rlActiveTextureSlot(2);
			rlEnableTexture(engine->gbuffer.albedoSpecTexture);
			rlActiveTextureSlot(3);
			rlEnableTexture(engine->gbuffer.zTexture);
			rlLoadDrawQuad();
			rlDisableShader();
			rlEnableColorBlend();
			EndMode3D();

			glBindFramebuffer(RL_READ_FRAMEBUFFER, engine->gbuffer.framebuffer);
			glBindFramebuffer(RL_DRAW_FRAMEBUFFER, 0);
			rlBlitFramebuffer(0, 0, screen_width, screen_height, 0, 0, screen_width, screen_height, GL_DEPTH_BUFFER_BIT);
			rlDisableFramebuffer();

			BeginMode3D(engine->camera);
			rlEnableShader(rlGetShaderIdDefault());

			DrawBoundingBox(engine->player.bound, RED);

			rlDisableShader();
			EndMode3D();

			DrawText("FINAL RESULT", 10, screen_height - 30, 20, DARKGREEN);
			break;
		}
		case (DEFERRED_POSITION): {
			DrawTextureRec((Texture2D){
			.id = engine->gbuffer.positionTexture,
			.width = screen_width,
			.height = screen_height,
			}, (Rectangle) { 0, 0, (float)screen_width, (float)-screen_height }, Vector2Zero(), WHITE);

			DrawText("POSITION TEXTURE", 10, screen_height - 30, 20, DARKGREEN);
			break;
		}
		case (DEFERRED_NORMAL): {
			DrawTextureRec((Texture2D){
			.id = engine->gbuffer.normalTexture,
			.width = screen_width,
			.height = screen_height,
			}, (Rectangle) { 0, 0, (float)screen_width, (float)-screen_height }, Vector2Zero(), WHITE);

			DrawText("NORMAL TEXTURE", 10, screen_height - 30, 20, DARKGREEN);
			break;
		}
		case (DEFERRED_ALBEDO): {
			DrawTextureRec((Texture2D){
			.id = engine->gbuffer.albedoSpecTexture,
			.width = screen_width,
			.height = screen_height,
			}, (Rectangle) { 0, 0, (float)screen_width, (float)-screen_height }, Vector2Zero(), WHITE);

			DrawText("ALBEDO TEXTURE", 10, screen_height - 30, 20, DARKGREEN);
			break;
		}
		case (DEFERRED_Z): {
			DrawTextureRec((Texture2D){
			.id = engine->gbuffer.zTexture,
			.width = screen_width,
			.height = screen_height,
			}, (Rectangle) { 0, 0, (float)screen_width, (float)-screen_height }, Vector2Zero(), WHITE);

			DrawText("Z TEXTURE", 10, screen_height - 30, 20, DARKGREEN);
			break;
		}
		default:break;
	};
	draw_ui(engine->player);
	EndDrawing();
}
