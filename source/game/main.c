#include "game.h"

typedef enum {
	ENGINE_STATE_GAME,
	ENGINE_STATE_MENU_START,
	ENGINE_STATE_LOADING_SCREEN,
	ENGINE_STATE_MENU_SETTING,
	ENGINE_STATE_MENU4,
	ENGINE_STATE_MENU5,
	ENGINE_STATE_MENU6,
	ENGINE_STATE_MENU7
}	engine_state_enum;

struct {
	bool		exit_window;
	bool		exit_request;
	int		engine_state;
	int		deferred_mode;
	unsigned int	width;
	unsigned int	height;
	Shader		*shader;
	gbuffer_t	gbuffer;
	Font		font;
	Camera3D	camera;
}	ctx;

int main(const int ac, char *av[]) {
	haven_memory_system_init();
	haven_thread_mgr_init();
	haven_time_init();

	ctx.height = 1080;
	ctx.width = 1920;

	ctx.deferred_mode = DEFERRED_SHADING;

	ctx.engine_state = ENGINE_STATE_LOADING_SCREEN;

	ctx.camera = (Camera3D){0};
	ctx.camera.position = (Vector3){ 2.0f, 3.0f, 2.0f };    // Camera position
	ctx.camera.target = (Vector3){ 0.0f, 1.0f, 0.0f };      // Camera looking at point
	ctx.camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
	ctx.camera.fovy = 45.0f;                                // Camera field-of-view Y
	ctx.camera.projection = CAMERA_PERSPECTIVE;             // Camera projection type(Camera3D) {

	haven_memory_system_print();
	InitAudioDevice();
	assert(IsAudioDeviceReady());

	InitWindow(ctx.width, ctx.height, "World of Haven : Chaos dungeons");
	assert(IsWindowReady());
	SetWindowState(FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_UNDECORATED);

	ctx.shader = game_shader_load();

	ctx.font = LoadFont("assets/Monster_hunter.ttf");

	SetTargetFPS(ac == 2 ? atoi(av[1]) : 60);

	ctx.gbuffer = haven_gbuffer_init(ctx.width, ctx.height);
	haven_deferred_set_loc(ctx.shader[SHADER_DEFERRED]);
	
	Model cube = LoadModelFromMesh(GenMeshCube(2.0f, 2.0f, 2.0f));
	cube.materials[0].shader = ctx.shader[SHADER_GBUFFER];
	
	FilePathList sounds_files = {0};
	sounds_files = LoadDirectoryFiles("assets/not_free/sound");
	haven_sound_init((const char**)sounds_files.paths, sounds_files.count);
	UnloadDirectoryFiles(sounds_files);

	FilePathList music_files = {0};
	music_files = LoadDirectoryFiles("assets/not_free/music");
	haven_music_init((const char**)music_files.paths, music_files.count);
	UnloadDirectoryFiles(music_files);

	rlEnableDepthTest();
	rlEnableBackfaceCulling();
	
	while (!WindowShouldClose()) {
		if (IsKeyPressed(KEY_SPACE)) {
			haven_sound_play(1);
		}
		if (IsKeyPressed(KEY_Q)) {
			ctx.deferred_mode++;
			ctx.deferred_mode %= 5;
		}
		switch (ctx.engine_state) {
			case (ENGINE_STATE_GAME): {
				scene_render(cube, &ctx.camera, ctx.gbuffer, ctx.shader, ctx.deferred_mode);
				break;
			}
			case (ENGINE_STATE_MENU_START): {
				BeginDrawing();
				ClearBackground(BLACK);
				switch (game_menu_start()) {
					case (1): {
						ctx.engine_state = ENGINE_STATE_GAME;
						HideCursor();
						break;
					}
					case (2): {
						ctx.engine_state = ENGINE_STATE_GAME;
						HideCursor();
						break;
					}
					case (3): {
						ctx.engine_state = ENGINE_STATE_MENU_SETTING;
						break;
					}
					case (4): {
						goto close;
						break;
					}
					default:break;
				}
				EndDrawing();
				break;
			}
			case (ENGINE_STATE_LOADING_SCREEN): {
				game_loading_screen();
				ctx.engine_state = ENGINE_STATE_MENU_START;
				break;
			}
			case (ENGINE_STATE_MENU_SETTING): {
				BeginDrawing();
				ClearBackground(BLACK);
				//setting
				EndDrawing();
				break;
			}
			case (ENGINE_STATE_MENU4): {
				BeginDrawing();
				ClearBackground(BLACK);
				EndDrawing();
				break;
			}
			case (ENGINE_STATE_MENU5): {
				BeginDrawing();
				ClearBackground(BLACK);
				EndDrawing();
				break;
			}
			case (ENGINE_STATE_MENU6): {
				BeginDrawing();
				ClearBackground(BLACK);
				EndDrawing();
				break;
			}
			case (ENGINE_STATE_MENU7): {
				BeginDrawing();
				ClearBackground(BLACK);
				EndDrawing();
				break;
			}
			default:{
				break;
			}
		}
		haven_music_update();
		haven_time_update();
		haven_stack_reset();
	}
	close:
	haven_music_close();
	haven_sound_close();
	CloseAudioDevice();
	game_shader_unload(ctx.shader);
	ShowCursor();
	haven_thread_mgr_close();
	haven_memory_system_shutdown();
	CloseWindow();
	return(0);
}
