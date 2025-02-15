#ifndef PROTOTYPE_HPP
# define PROTOTYPE_HPP

#include "engine.h"

engine_t	init_engine(void);
void	close_engine(engine_t *engine);

void	voxel_render(engine_t *engine, world_t *world);

void	update_input(engine_t *engine);

void	draw_inventory(inventory_t inventory);
void	init_toolbar(tool_bar_t *tool_bar);
void	draw_toolbar(tool_bar_t *tool_bar);

void	setup_world_vao(world_t *world);
void	reload_world_vao(world_t *world);

void	set_block(chunk_t *chunk, int x, int y, int z, int id);

void	draw_ui(sv_player_t player);

Texture2D	gen_texture_noise(fnl_state *noise);

#endif
