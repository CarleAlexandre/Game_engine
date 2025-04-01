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

static struct {
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
	Texture2D	*textures;
	unsigned int	textures_size;
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
	// ctx.camera.position = (Vector3){ 2.0f, 3.0f, 2.0f };// Camera position
	// ctx.camera.target = (Vector3){ 0.0f, 1.0f, 0.0f };  // Camera looking at point
	// ctx.camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };      // Camera up vector (rotation towards target)
	// ctx.camera.fovy = 45.0f;                            // Camera field-of-view Y
	// ctx.camera.projection = CAMERA_PERSPECTIVE;         // Camera projection type(Camera3D) {

	ctx.camera.fovy = 45.0f;
	ctx.camera.projection = CAMERA_PERSPECTIVE;
	ctx.camera.position = (Vector3){40, 40, 40};
	ctx.camera.up = (Vector3){0, 1, 0};
	ctx.camera.target = (Vector3){0, 0, 0};

	haven_memory_system_print();
	InitAudioDevice();
	assert(IsAudioDeviceReady());

	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_UNDECORATED | FLAG_WINDOW_RESIZABLE);//need to see if it work
	InitWindow(ctx.width, ctx.height, "World of Haven : Chaos dungeons");
	assert(IsWindowReady());
	rlEnableSmoothLines();//need to see if it work
	
	ctx.shader = game_shader_load();

	ctx.font = LoadFont("assets/Monster_hunter.ttf");

	SetTargetFPS(ac == 2 ? atoi(av[1]) : 60);

	ctx.gbuffer = haven_gbuffer_init(ctx.width, ctx.height);
	haven_deferred_set_loc(ctx.shader[SHADER_DEFERRED]);
	
	Model cube = haven_skybox_create(ctx.shader[SHADER_SKYBOX]);
	
	FilePathList sounds_files = {0};
	sounds_files = LoadDirectoryFiles("assets/sound");
	haven_sound_init((const char**)sounds_files.paths, sounds_files.count);
	UnloadDirectoryFiles(sounds_files);

	FilePathList music_files = {0};
	music_files = LoadDirectoryFiles("assets/not_free/music");
	haven_music_init((const char**)music_files.paths, music_files.count);
	UnloadDirectoryFiles(music_files);

	FilePathList texture_files = {0};
	texture_files = LoadDirectoryFiles("assets/textures");
	ctx.textures = malloc(sizeof(Texture2D) * texture_files.count);
	for (int i = 0; i < texture_files.count; i++) {
		ctx.textures[i] = LoadTexture(texture_files.paths[i]);
	}
	ctx.textures_size = texture_files.count;
	UnloadDirectoryFiles(texture_files);

	game_menu_start_init();

	rlEnableDepthTest();
	rlEnableBackfaceCulling();
	
	while (!WindowShouldClose()) {
		switch (ctx.engine_state) {
			case (ENGINE_STATE_GAME): {
				scene_render(cube, &ctx.camera, ctx.gbuffer, ctx.shader, ctx.deferred_mode);
				break;
			}
			case (ENGINE_STATE_MENU_START): {
				
				BeginDrawing();
				ClearBackground(BLACK);
				switch (game_menu_start_update()) {
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
				game_menu_start_render(ctx.textures, ctx.font);
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
		// haven_music_update();
		haven_time_update();
		haven_stack_reset();
	}
	close:

	game_menu_start_clear();
	for (int i = 0; i < ctx.textures_size; i++) {
		UnloadTexture(ctx.textures[i]);
	}
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
