#ifndef GAME_H
# define GAME_H

#include "engine/core.h"
#include "engine/render.h"
#include "engine/audio.h"

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

#define MAX_SHADER 6

Shader	*game_shader_load(void);
void	game_shader_unload(Shader *shader);
void	scene_render(const Model cube, Camera3D *camera, const gbuffer_t gbuffer, const Shader shader[MAX_SHADER], int deferred_mode);
int	game_menu_start(void);
void	game_loading_screen(void);

#endif