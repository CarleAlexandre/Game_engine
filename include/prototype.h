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

void	ExtractFrustumFromMatrix(Matrix mat, Frustum *frustum);
bool	IsBoxInFrustum(BoundingBox box, Frustum frustum);

void		gen_chunk_render(chunk_mesh_t *mesh);
void		voxel_render(engine_t *engine, world_t *world);
gbuffer_t	loadGbuffer(int width, int height, Shader deffered_shader);
world_mesh_t	assemble_world_mesh(chunk_t *rqueue[128], unsigned int rcount);
world_render_t	gen_world_render(world_mesh_t *mesh);

/*
	VOXEL
*/

void	gen_world_mesh(world_t *world, engine_t *engine);
void	update_world_render(world_t *world, engine_t *engine);
void	setup_world_render(world_t *world, engine_t *engine);

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
