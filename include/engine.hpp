#ifndef ENGINE_HPP
# define ENGINE_HPP

#include <vector>
#include <raylib.h>

typedef struct s_player {
	Vector3 pos;
}	player_t;

typedef struct s_object {
	Vector3 pos;
	float scale;
	int type;
}	object_t;

typedef struct s_item {

}	item_t;

typedef struct s_entity {
	Vector3 pos;
	int size;
	int layer ;
}	entity_t;

typedef struct s_wall {
	Vector3 p1;
	Vector3 p2;
	int layer;
}	wall_t;

typedef struct s_light {
	Vector3 pos;
	int type;
	int intensity;
	int pos_loc;
	int type_loc;
	int intensity_loc;
}	light_t;

typedef struct s_terrain {
	Model model;
	Vector3 pos;
	float scale;
}	terrain_t;

typedef struct s_level {
	terrain_t terrain;
	std::vector<object_t> objs;
	std::vector<light_t> lights;
}	level_t;

typedef struct s_engine {
	RenderTexture fbo;
	Shader posprocess;
	Shader light;
	Camera3D camera;
}	engine_t;

#endif