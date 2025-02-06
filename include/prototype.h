#ifndef PROTOTYPE_HPP
# define PROTOTYPE_HPP

#include <engine.h>

engine_t	init_engine(void);
void	close_engine(engine_t *engine);

void	UpdateLightValues(Shader shader, light_t light);
light_t	CreateLight(int type, Vector3 position, Vector3 target, Color color, Shader shader);
void	render(engine_t *engine, chunk_t *world[5][5]);

void	update_input(engine_t *engine);


void	draw_inventory(inventory_t inventory);
void	init_toolbar(tool_bar_t *tool_bar);
void	draw_toolbar(tool_bar_t *tool_bar);

void	heightmap_voxel(Image heightmap);
void	generate_chunk_mesh(engine_t *engine, chunk_t *chunk);
chunk_t	*generate_terrain(Vector2 chunk_pos);
void render_mesh(chunk_t *chunk);

void	draw_ui(sv_player_t player);

#endif