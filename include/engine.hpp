#ifndef ENGINE_HPP
# define ENGINE_HPP

#include <vector>

typedef struct s_point {
	float x, y;
}	point_t ;

typedef struct s_player {
	point_t pos;
}	player_t;

typedef struct s_object {
	point_t pos;
	float rad;
}	object_t;

typedef struct s_item {
	point_t pos;
}	item_t;

typedef struct s_entity {
	point_t pos;
	int size;
}	entity_t;

typedef struct s_wall {
	point_t p1;
	point_t p2;
}	wall_t;

typedef struct s_light {
	point_t pos;
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

typedef struct s_scene {
	std::vector<player_t*> players;
	std::vector<object_t*> objects;
	std::vector<entity_t*> entities;
	std::vector<wall_t*> walls;
}	scene_t;

#endif