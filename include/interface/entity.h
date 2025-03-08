#ifndef ENTITY_SYSTEM
# define ENTITY_SYSTEM

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
}	entity_animation_t;

typedef struct	s_entity {
	entity_stats_t*		stats;
	entity_info_t*		info;
	entity_animation_t*	anim;
}	entity_t;

/**
 * @brief 
 * 
 */
void	init_entities(void);

bool		add_entity();
entity_t	*get_entity();
void		del_entity(unsigned int idx);

#endif