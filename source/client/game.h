#ifndef GAME_H
# define GAME_H

#include "../engine/allocator.h"
#include "../engine/thread.h"
#include "render.h"
#include "audio.h"

#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>
#include <rcamera.h>
// #define RLIGHTS_IMPLEMENTATION
// // #include <rlights.h>

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
	SHADER_SOBEL,
	SHADER_PBR
}	shader_type_enum;

typedef enum {
	TEXTURE_GUI_BUTTON,
}	texture_name_enum;

// typedef enum {
// 	,
// }	music_name_enum;

typedef enum {
	SOUND_DUMMY1,
	SOUND_ITEM_FOUND,
	SOUND_DUMMY2,
	SOUND_MENU_CLICK1,
	SOUND_MENU_CLICK2,
	SOUND_MENU_CLICK3
}	sound_name_enum;

#define MAX_SHADER 6

Shader	*game_shader_load(void);
void	game_shader_unload(Shader *shader);
void	scene_render(const Model skybox, Camera3D *camera, const gbuffer_t gbuffer, const Shader shader[MAX_SHADER], int deferred_mode);

void	game_loading_screen(void);

/*
	game menu
*/

void	game_menu_start_init(void);
int	game_menu_start_update();
void	game_menu_start_render(Texture2D *texture, Font font);
void	game_menu_start_clear(void);

#endif