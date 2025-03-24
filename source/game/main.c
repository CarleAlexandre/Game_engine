#include "game.h"
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>


void	hot_reload_game() {

}

struct {
	deferred_mode_enum deferred_mode;
	Camera3D camera;
	Shader	*shader;
	gbuffer_t gbuffer;
	unsigned int width;
	unsigned int height;
}	ctx;

int main(const int ac, char *av[]) {
	haven_memory_system_init();
	haven_thread_mgr_init();
	haven_time_init();

	ctx.height = 1080;
	ctx.width = 1920;

	ctx.deferred_mode = DEFERRED_SHADING;

	ctx.camera = (Camera3D){0};
	ctx.camera.position = (Vector3){ 2.0f, 3.0f, 2.0f };    // Camera position
	ctx.camera.target = (Vector3){ 0.0f, 1.0f, 0.0f };      // Camera looking at point
	ctx.camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	ctx.camera.fovy = 45.0f;                                // Camera field-of-view Y
	ctx.camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type(Camera3D) {

	haven_memory_system_print();
	InitWindow(ctx.width, ctx.height, "World of Haven : Chaos dungeons");
	SetWindowState(FLAG_BORDERLESS_WINDOWED_MODE | FLAG_WINDOW_RESIZABLE);

	ctx.shader = game_shader_load();

	HideCursor();
	SetTargetFPS(ac == 2 ? atoi(av[1]) : 60);

	ctx.gbuffer = haven_gbuffer_init(ctx.width, ctx.height);
	haven_deferred_set_loc(ctx.shader[SHADER_DEFERRED]);
	
	Model cube = LoadModelFromMesh(GenMeshCube(2.0f, 2.0f, 2.0f));
	cube.materials[0].shader = ctx.shader[SHADER_GBUFFER];
	
	rlEnableDepthTest();
	rlEnableBackfaceCulling();

	bool pause = false;

	while (!WindowShouldClose()) {

		if (IsKeyPressed(KEY_SPACE)) {
			pause = !pause;
		}

		while (pause) {
			haven_time_usleep(1000);
			if (IsKeyPressed(KEY_SPACE)) {
				pause = !pause;
			}
		}

		if (IsKeyPressed(KEY_Q)) {
			ctx.deferred_mode++;
			ctx.deferred_mode %= 5;
		}

		UpdateCamera(&ctx.camera, CAMERA_FREE);

		float camerapos[3] = {ctx.camera.position.x, ctx.camera.position.y, ctx.camera.position.z};

		SetShaderValue(ctx.shader[SHADER_DEFERRED], ctx.shader[SHADER_DEFERRED].locs[SHADER_LOC_VECTOR_VIEW], camerapos, SHADER_UNIFORM_VEC3);
		//update_input

		//launch all thread task

		//wait all thread task
		
		BeginDrawing(); {
			haven_gbuffer_start_draw(ctx.gbuffer, ctx.camera, ctx.shader[SHADER_GBUFFER]);
			//drawDeferred scene here

			DrawModel(cube, Vector3Zero(), 1.0f, WHITE);

			haven_gbuffer_end_draw();

			switch (ctx.deferred_mode){
				case (DEFERRED_SHADING) : {
					haven_gbuffer_rendering(ctx.gbuffer, ctx.camera, ctx.shader[SHADER_DEFERRED]);
					BeginMode3D(ctx.camera); {

						rlEnableShader(rlGetShaderIdDefault());

						DrawCube((Vector3){10, 10, 10}, 10, 10, 10, RED);

						rlDisableShader();
					}EndMode3D();
					break;
				}
				case (DEFERRED_POSITION) : {
					haven_gbuffer_texture_render(ctx.gbuffer.positionTexture, ctx.gbuffer.width, ctx.gbuffer.height);
					break;
				}
				case (DEFERRED_NORMAL) : {
					haven_gbuffer_texture_render(ctx.gbuffer.normalTexture, ctx.gbuffer.width, ctx.gbuffer.height);
					break;
				}
				case (DEFERRED_ALBEDO) : {
					haven_gbuffer_texture_render(ctx.gbuffer.albedoSpecTexture, ctx.gbuffer.width, ctx.gbuffer.height);
					break;
				}
				case (DEFERRED_ZBUFFER) : {
					haven_gbuffer_texture_render(ctx.gbuffer.zTexture, ctx.gbuffer.width, ctx.gbuffer.height);
					break;
				}
				
				default:{
					break;
				}
			}
			
			DrawFPS(10, 10);
		} EndDrawing();

		haven_time_update();
		haven_stack_reset();
	}

	game_shader_unload(ctx.shader);
	ShowCursor();
	haven_thread_mgr_close();
	haven_memory_system_shutdown();
	CloseWindow();
	return(0);
}
