#include "engine/core.h"
#include "engine/render.h"

#include <rlgl.h>
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>


void	hot_reload_game() {

}

// Deferred mode passes
typedef enum {
	DEFERRED_POSITION,
	DEFERRED_NORMAL,
	DEFERRED_ALBEDO,
	DEFERRED_ZBUFFER,
	DEFERRED_SHADING
}	deferred_mode_enum;

typedef enum {
	SHADER_GBUFFER,
	SHADER_DEFERRED,
	SHADER_SKYBOX,
	SHADER_VOXEL,
	SHADER_SOBER,
	SHADER_PBR
}	shader_type_enum;

#define MAX_SHADER 6

struct {
	deferred_mode_enum deferred_mode;
	Camera3D camera;
	Shader	shader[MAX_SHADER];
	gbuffer_t gbuffer;
	unsigned int width;
	unsigned int height;
}	ctx;

int main(const int ac, char *av[]) {
	haven_memory_system_init();
	haven_thread_mgr_init();
	haven_time_init();

	haven_memory_system_print();
	InitWindow(1920, 1080, "World of Haven : Chaos dungeons");
	SetWindowState(FLAG_BORDERLESS_WINDOWED_MODE | FLAG_WINDOW_RESIZABLE);

	HideCursor();
	SetTargetFPS(ac == 2 ? atoi(av[1]) : 60);

	ctx.gbuffer = haven_gbuffer_init(ctx.width, ctx.height, ctx.shader[SHADER_GBUFFER].id);

	rlEnableDepthTest();
	rlEnableBackfaceCulling();

	bool pause = false;

	while (!WindowShouldClose()) {

		while (pause) {
			haven_time_usleep(1000);
			if (IsKeyPressed(KEY_SPACE)) {
				pause = !pause;
			}
		}

		//update_input

		//launch all thread task

		//wait all thread task
		
		BeginDrawing(); {
			ClearBackground(RAYWHITE);
			haven_gbuffer_start_draw(ctx.gbuffer, ctx.camera, ctx.shader[SHADER_GBUFFER]);
			//drawDeferred scene here

			haven_gbuffer_end_draw();

			switch (ctx.deferred_mode){
				case (DEFERRED_SHADING) : {
					haven_gbuffer_rendering(ctx.gbuffer, ctx.camera, ctx.shader[SHADER_DEFERRED]);
					BeginMode3D(ctx.camera); {

						rlEnableShader(rlGetShaderIdDefault());
						//forward rendering here
						
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
	
	ShowCursor();
	haven_thread_mgr_close();
	haven_memory_system_shutdown();
	CloseWindow();
	return(0);
}
