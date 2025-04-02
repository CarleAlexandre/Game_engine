#ifndef HAVEN_ENTITY_H
# define HAVEN_ENTITY_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <raylib.h>

typedef struct s_stats {
	int		health;
	uint32_t	max_health;
	unsigned short 	regeneration;
	unsigned short	damage;
}	entity_stats_t;// change with buff or equipement

typedef struct	s_entity_info {
	Vector3	position;
	Vector3	min;
	Vector3	max;
	Vector3	direction;
	Vector3	velocity;
}	entity_info_t;

typedef struct	s_entity_animation {
//bones, mesh etc etc
	int	size;
	int	model_id;
}	entity_animation_t;

typedef struct	s_entity {
	entity_stats_t stats;
	entity_info_t info;
	entity_animation_t anim;
}	entity_t;



/**
 * @brief initialize entities dynamic array
 * 
 */
void	init_entities(void);

/**
 * @brief add entity to list
 * 
 * @param entity 
 * @return uint32_t 
 */
uint32_t	add_entity(entity_t entity);

/**
 * @brief Get a copy of the entity
 * 
 * @param idx 
 * @return entity_t 
 */
entity_t	get_entity(unsigned int idx);

/**
 * @brief delete entity at index idx
 * 
 * @param idx 
 */
void		del_entity(unsigned int idx);

#endif