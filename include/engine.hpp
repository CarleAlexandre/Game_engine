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
}	object_t;

typedef struct s_item {
	point_t pos;
}	item_t;

typedef struct s_entity {
	point_t pos;
}	entity_t;

typedef struct s_wall {
	point_t pos;
}	wall_t;

typedef struct s_scene {
	std::vector<player_t> players;
	std::vector<object_t> objects;
	std::vector<entity_t> entities;
	std::vector<wall_t> walls;
}	scene_t;

#endif