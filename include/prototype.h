#ifndef PROTOTYPE_HPP
# define PROTOTYPE_HPP

#include "engine.h"

/*
	ENGINE
*/

void		close_engine(engine_t *engine);
engine_t	init_engine(void);

/*
	INPUT
*/

void	update_input(engine_t *engine,  world_t *world);

/*
	INVENTORY
*/



/*
	RENDER
*/

void		setup_world_render(rend_pip_t *pipeline);
void		reload_world_render(rend_pip_t *pipeline);

void		voxel_render(engine_t *engine, world_t *world);
gbuffer_t	loadGbuffer(int width, int height, Shader deffered_shader);

/*
	VOXEL
*/

void	reload_voxel_world(world_t *world, engine_t *engine);
void	set_block(chunk_t *chunk, int x, int y, int z, voxel_t *vox);
void	gen_render_chunk(world_t *world, engine_t *engine);

/*
	UI
*/

void	draw_ui(sv_player_t player);
void	draw_inventory(inventory_t inventory);
void	init_toolbar(tool_bar_t *tool_bar);
void	draw_toolbar(tool_bar_t *tool_bar);

/*
	PROCEDURAL
*/

Texture2D	gen_texture_noise(fnl_state *noise);
chunk_t		**chunk_gen_height(int x_off, int z_off, int *size, fnl_state *noise);

#endif
