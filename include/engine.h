#ifndef ENGINE_HPP
# define ENGINE_HPP

# include <glad.h>
# include <ext/FastNoiseLite.h>

# include <raylib.h>
# include <rlgl.h>
# include <raymath.h>
# include <rcamera.h>

# include "sparse_octree.h"
# include "dynamic_array.h"

# include <stdbool.h>
# include <stdint.h>
# include <stddef.h>
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <assert.h>

/*
	DEFINE
*/

# define GRAY_VALUE(c) ((float)(c.r + c.g + c.b)/3.0f)

# define FACE_TOP    0
# define FACE_BOTTOM 1
# define FACE_LEFT   2
# define FACE_RIGHT  3
# define FACE_FRONT  4
# define FACE_BACK   5

/* 
	TYPEDEF
*/

// typedef enum {
// 	DEFERRED_POSITION,
// 	DEFERRED_NORMAL,
// 	DEFERRED_ALBEDO,
// 	DEFERRED_SHADING,
// 	DEFERRED_Z,
// } deferred_mode;

typedef enum {
	item_est,
}	item_identifier;

typedef enum {
	model_test,
}	model_identifier;

typedef enum {
	item_type_weapon,
	item_type_tool,
	item_type_material,
	item_type_structure,
	item_type_consumable,
}item_type_e;

typedef enum item{
	tool_shovel,
	tool_axe,
	tool_hoe,
	tool_sprinkler,
	tool_wrench,
	tool_lockpick,
	weapon_battle_axe,
	weapon_lance,
	weapon_bow,
	weapon_bowgun,
	weapon_wonder_weapon,
	weapon_lost_knowledge,
	weapon_fist,
	weapon_magic_staff,
	consumable_bandage,
	consumable_kit,
	consumable_food,
	consumable_drugs,
	consumable_potion,
	consumable_music_instrument,
	consumable_trap,
	consumable_bomb,
	consumable_scroll,
	material_wood,
	material_sand,
	material_stone,
	material_cement,
	material_iron,
	material_silver,
	material_mithril,
	material_orichalc,
	material_malachit,
}	item_enum;

typedef enum {
	shader_voxel_solid,
	shader_voxel_liquid,
	shader_voxel_transparent,
	shader_pbr,
	shader_light,
	shader_gbuffer,
	shader_deffered,
	shader_sobel,
}	shader_enum;

typedef enum {
	item_id,
	item_type,
	item_stats,
}	token_type_enum;

typedef enum {
	error_dummy = 0,
	error_file_not_found = 1,
	error_file_empty = 2,
	error_file_exist = 3,
	error_file_corrupted = 4,
}	error_enum;

typedef enum {
	item_filetype,
	object_filetype,
	world_filetype,
	player_filetype,
}	filetype_enum;

typedef enum {
	block_air,
	block_dirt,
	block_sand,
	block_stone,
	block_water,
}	block_id_enum;

/* 
	DATA STRUCT
*/

typedef struct s_token {
	int	id;
	char	*data;
} t_token;

typedef	struct s_file {
	unsigned char	*data;
	int		size;
	filetype_enum	type;
}	file_t;

typedef struct s_stats {
	int	health;
	int	max_health;
}	stats_t;

typedef struct s_player {
	Vector3	pos;
	BoundingBox	bound;
}	player_t;

typedef struct s_object {
	Vector3	pos;
	float	scale;
	int	type;
	BoundingBox	bound;
	bool	render;
	int	model_id;
}	object_t;

typedef struct s_item_stats {
	int	material;
	int	damage;
	int	durability;
	int	effect;
	int	rarity;
}	item_stats_t;

typedef struct item_data {
	int	type;
	int	texture_id;
	int	max_stack;
	int	size;
}	item_data_t;

typedef struct s_item {
	item_data_t	*data;
	object_t	*obj;
	item_stats_t	*stats;
}	item_t;

typedef	struct s_inventory {
	int		money;
	item_t		*item;
	unsigned int	size;
}	inventory_t;

typedef struct s_tool_bar{
	int	current_item;
	bool	gotonext;
	bool	gotoprev;
	Vector2	pos1;
	Vector2	pos2;
	Vector2	topos1;
	Vector2	topos2;
}	tool_bar_t;

typedef struct s_projectile {
	Ray	shot;
	float	mass;
	int	model_id;
	int	lifespan;
}	projectile_t;

typedef struct s_entity {
	Vector3		pos;
	int		size;
	int		layer;
	int		model_id;
	BoundingBox	bounding_box;
}	entity_t;

typedef struct	sv_player_s {
	bool			show_inventory;
	stats_t			stats;
	Vector3			pos;
	tool_bar_t		toolbar;
	BoundingBox		bound;
	inventory_t		inventory;
	unsigned long long	uid;
}	sv_player_t;

/*
	VOXEL RENDER
*/

typedef struct	s_face_data {
	int		block_id;
	int		face_data;
}	face_data_t;

typedef struct	s_chunk_render {
	uint32_t	face_offset;
	uint32_t	face_count;
	int		x, y, z;
}	chunk_render_t;

typedef struct	s_world_render {
	unsigned int	vao, vbo, ssbo;
	dyn_array_t	*faces;//keep face_data_t *
	dyn_array_t	*rqueue;//keep chunk_render_t*
}	world_render_t;

/*
	VOXEL
*/

typedef struct s_voxel {
	unsigned short	block_id;
	float		height;
	float		pressure;
	short		stats;
}	voxel_t;

typedef struct	s_chunk {
	svo_t		*blocks;
	BoundingBox	bounding_box;
}	chunk_t;

typedef struct	s_world {
	svo_t		*tree;
}	world_t;

/*
	ENGINE
*/

typedef struct s_draw_arrays_indirect_command{
	uint32_t	count;// Number of faces
	uint32_t	instanceCount;// Number of instances to draw
	uint32_t	first;// Offset into the VBO (in face)
	uint32_t	baseInstance;// Instance id
}	indirect_cmd_t;

typedef struct s_gbuffer{
	unsigned int	framebuffer;
	unsigned int	positionTexture;
	unsigned int	normalTexture;
	unsigned int	albedoSpecTexture;
	unsigned int	depthRenderbuffer;
	unsigned int	zTexture;
}	gbuffer_t;

typedef struct s_render_pipeline {
	gbuffer_t	gbuffer;
	GLuint		indirect_buffer;
	world_render_t	world;
}	rend_pip_t;

typedef struct s_engine {
	Shader		shader[8];
	Camera3D	camera;
	rend_pip_t	render;
	bool		debug;
	sv_player_t	player;
}	engine_t;

#endif
