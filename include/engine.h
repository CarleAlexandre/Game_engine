#ifndef ENGINE_HPP
# define ENGINE_HPP

# include <glad.h>
# include <extern/FastNoiseLite.h>

# define GRAPHICS_API_OPENGL_43
# include <raylib.h>
# include <rlgl.h>
# include <raymath.h>
# include <rcamera.h>

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

/* 
	TYPEDEF
*/

typedef enum {
	DEFERRED_POSITION,
	DEFERRED_NORMAL,
	DEFERRED_ALBEDO,
	DEFERRED_SHADING,
	DEFERRED_Z,
} deferred_mode;

typedef enum {
	item_test,
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
	shader_cubemap,
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

#endif
