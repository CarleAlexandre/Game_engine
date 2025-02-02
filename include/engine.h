#ifndef ENGINE_HPP
# define ENGINE_HPP

#define GLSL_VERSION 330
#include <raylib.h>
#include <glad.h>
#include <rlgl.h>
#include <raymath.h>
#include <rcamera.h>
#include <stdlib.h>

/*
	DEFINE
*/

#define MAX_LIGHTS  4
#define RL_READ_FRAMEBUFFER                     0x8CA8      // GL_READ_FRAMEBUFFER
#define RL_DRAW_FRAMEBUFFER                     0x8CA9      // GL_DRAW_FRAMEBUFFER

#define GRAY_VALUE(c) ((float)(c.r + c.g + c.b)/3.0f)

#define ITEM_MAGIC 

/* 
	TYPEDEF
*/

typedef enum {
    LIGHT_DIRECTIONAL = 0,
    LIGHT_POINT
} LightType;

typedef enum {
	DEFERRED_POSITION,
	DEFERRED_NORMAL,
	DEFERRED_ALBEDO,
	DEFERRED_SHADING,
	DEFERRED_Z,
} deferred_mode;
 
typedef enum {
	GLOBAL_LIGHT,
	DIRECTIONNAL_LIGHT,
}	light_type;

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

enum tool{
	tool_shovel,
	tool_axe,
	tool_hoe,
	tool_sprinkler,
	tool_wrench,
	tool_lockpick,
};

enum weapon{
	weapon_battle_axe,
	weapon_lance,
	weapon_bow,
	weapon_bowgun,
	weapon_wonder_weapon,
	weapon_lost_knowledge,
	weapon_fist,
	weapon_magic_staff,
};

enum consumable {
	consumable_bandage,
	consumable_kit,
	consumable_food,
	consumable_drugs,
	consumable_potion,
	consumable_music_instrument,
	consumable_trap,
	consumable_bomb,
	consumable_scroll,
};

enum structure {
	machine_pc,
	machine_door,
	structure_wall,
	structure_brick,
	structure_fence,
};

enum material {
	material_wood,
	material_sand,
	material_stone,
	material_cement,
	material_iron,
	material_silver,
	material_mithril,
	material_orichalc,
	material_malachit,
};

typedef enum {
	item_id,
	item_type,
	item_stats,
} token_type_e;

typedef enum {
	error_dummy = 0,
	error_file_not_found = 1,
	error_file_empty = 2,
	error_file_exist = 3,
	error_file_corrupted = 4,
} error_e;

typedef enum {
	item_filetype,
	object_filetype,
	world_filetype,
	player_filetype,
} filetype_e;

/* 
	DATA STRUCT
*/

typedef struct s_token {
	int id;
	char *data;
} t_token;

typedef	struct s_file {
	unsigned char	*data;
	int		size;
	filetype_e	type;
}	file_t;

typedef struct s_stats {
	int health;
	int max_health;
}	stats_t;

typedef struct s_player {
	Vector3 pos;
	BoundingBox bound;
}	player_t;

typedef struct s_object {
	Vector3 pos;
	float scale;
	int type;
	BoundingBox bound;
	bool render;
	int model_id;
}	object_t;

typedef struct s_item {
	bool is_placable;
	int type;
	int texture_id;
	int material;
	int damage;
	int durability;
	int effect;
	int max_stack;
	int rarity;
	int size;
}	item_t;

typedef	struct s_inventory {
	item_t *item;
	unsigned int size;
}	inventory_t;

typedef struct s_tool_bar{
	int current_item;
	bool gotonext;
	bool gotoprev;
	Vector2 pos1;
	Vector2 pos2;
	Vector2 topos1;
	Vector2 topos2;
}	tool_bar_t;

typedef struct s_projectile {
	Ray shot;
	float mass;
	int model_id;
	int lifespan;
}	projectile_t;

typedef struct s_entity {
	Vector3 pos;
	int size;
	int layer;
	int model_id;
	BoundingBox bound;
}	entity_t;

typedef struct sv_player_s {
	Vector3 pos;
	BoundingBox bound;
	stats_t stats;
	bool show_inventory;
	tool_bar_t toolbar;
	inventory_t inventory;
	unsigned long long uuid;
}	sv_player_t;

//

typedef struct s_light {
	int type;
	bool enabled;
	Vector3 position;
	Vector3 target;
	Color color;
	float attenuation;

	// Shader locations
	int enabledLoc;
	int typeLoc;
	int positionLoc;
	int targetLoc;
	int colorLoc;
	int attenuationLoc;
}	light_t;

typedef struct s_terrain {
	Model model;
	Vector3 pos;
	float scale;
	BoundingBox bound;
}	terrain_t;

typedef struct s_chunk {
	int *blocks;// 16 * 16 * 16;
}	chunk_t;
//chunk is only used for dungeon generation

typedef struct s_vox_vert {
	int data;
};

typedef enum {
	vert_pos_x_e =  0,
	vert_pos_y_e = 1 << 6,
	vert_pos_z_e = 1 << 12,
	vert_face_e = 1 << 18,
	vert_text_id_e = 1 << 21,
}	vox_vert_shift_e;

typedef struct s_gbuffer{
	unsigned int framebuffer;
	unsigned int positionTexture;
	unsigned int normalTexture;
	unsigned int albedoSpecTexture;
	unsigned int depthRenderbuffer;
	unsigned int zTexture;
}	gbuffer_t;

typedef struct s_engine {
	Shader posprocess;
	Shader gbuffer_shader;
	Shader deffered_shader;
	gbuffer_t gbuffer;
	Camera3D camera;
	RenderTexture2D fbo;
	deferred_mode mode;
	Model cube;
	Model Sphere;
	Model dummy;
	light_t lights[MAX_LIGHTS];
	sv_player_t player;
}	engine_t;

#endif