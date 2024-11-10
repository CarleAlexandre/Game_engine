#ifndef ENGINE_HPP
# define ENGINE_HPP

#include <vector>

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

typedef struct s_map {
	std::vector<object_t> objs;
	std::vector<entity_t> entity;
	std::vector<wall_t> walls;
}	map_t;

typedef struct s_terrain {
	Model model;
	Vector3 pos;
	float scale;
}	terrain_t;

typedef struct s_level {
	terrain_t terrain;

}	level_t;

typedef struct s_scene {
	std::vector<player_t*> players;
	std::vector<object_t*> objects;
	std::vector<entity_t*> entities;
	std::vector<wall_t*> walls;
}	scene_t;

#endif