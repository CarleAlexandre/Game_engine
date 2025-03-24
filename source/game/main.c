#include "game.h"
#define RAYGUI_IMPLEMENTATION
#include <raygui.h>

typedef enum {
	ENGINE_STATE_GAME,
	ENGINE_STATE_MENU1,
	ENGINE_STATE_MENU2,
	ENGINE_STATE_MENU3,
	ENGINE_STATE_MENU4,
	ENGINE_STATE_MENU5,
	ENGINE_STATE_MENU6,
	ENGINE_STATE_MENU7
}	engine_state_enum;

struct {
	Shader	*shader;
	gbuffer_t gbuffer;
	unsigned int width;
	unsigned int height;
	Font font;
	int engine_state;
	Camera3D camera;
	deferred_mode_enum deferred_mode;
}	ctx;

int main(const int ac, char *av[]) {
	haven_memory_system_init();
	haven_thread_mgr_init();
	haven_time_init();

	ctx.height = 1080;
	ctx.width = 1920;

	ctx.deferred_mode = DEFERRED_SHADING;

	ctx.engine_state = ENGINE_STATE_MENU1;

	ctx.camera = (Camera3D){0};
	ctx.camera.position = (Vector3){ 2.0f, 3.0f, 2.0f };    // Camera position
	ctx.camera.target = (Vector3){ 0.0f, 1.0f, 0.0f };      // Camera looking at point
	ctx.camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	ctx.camera.fovy = 45.0f;                                // Camera field-of-view Y
	ctx.camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type(Camera3D) {

	haven_memory_system_print();
	InitWindow(ctx.width, ctx.height, "World of Haven : Chaos dungeons");
	SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_UNDECORATED);

	ctx.shader = game_shader_load();

	ctx.font = LoadFont("assets/not_free/PirataOne-Regular.ttf");
	GuiSetFont(ctx.font);

	HideCursor();
	SetTargetFPS(ac == 2 ? atoi(av[1]) : 60);

	ctx.gbuffer = haven_gbuffer_init(ctx.width, ctx.height);
	haven_deferred_set_loc(ctx.shader[SHADER_DEFERRED]);
	
	Model cube = LoadModelFromMesh(GenMeshCube(2.0f, 2.0f, 2.0f));
	cube.materials[0].shader = ctx.shader[SHADER_GBUFFER];
	
	rlEnableDepthTest();
	rlEnableBackfaceCulling();
	
	while (!WindowShouldClose()) {
		
		if (IsKeyPressed(KEY_Q)) {
			ctx.deferred_mode++;
			ctx.deferred_mode %= 5;
		}
		switch (ctx.engine_state) {
			case (ENGINE_STATE_GAME): {
				scene_render(cube, &ctx.camera, ctx.gbuffer, ctx.shader, ctx.deferred_mode);
				break;
			}
			case (ENGINE_STATE_MENU1): {
				break;
			}
			case (ENGINE_STATE_MENU2): {
				break;
			}
			case (ENGINE_STATE_MENU3): {
				break;
			}
			case (ENGINE_STATE_MENU4): {
				break;
			}
			case (ENGINE_STATE_MENU5): {
				break;
			}
			case (ENGINE_STATE_MENU6): {
				break;
			}
			case (ENGINE_STATE_MENU7): {
				break;
			}
			default:{
				break;
			}
		}

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
