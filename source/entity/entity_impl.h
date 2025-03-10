#ifndef	ENTITY_IMPL
# define ENTITY_IMPL

//entity have pointer to copenent, when new entity, aloocate more space on needed copenent array and return pointer to set it into entity struct

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <cglm/cglm.h>

typedef struct s_stats {
	int		health;
	uint32_t	max_health;
	unsigned short 	regeneration;
	unsigned short	damage;
}	entity_stats_t;// change with buff or equipement

typedef struct	s_entity_info {
	vec3	position;
	vec3	min;
	vec3	max;
	vec3	direction;
	vec3	velocity;
}	entity_info_t;

typedef struct	s_entity_animation {
//bones, mesh etc etc
	int	size;
	int	model_id;
}	entity_animation_t;

typedef struct	s_entity {
	entity_stats_t*		stats;
	entity_info_t*		info;
	entity_animation_t*	anim;
}	entity_t;

#endif
