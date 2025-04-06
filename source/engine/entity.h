#ifndef ENTITY_H
# define ENTITY_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <raylib.h>

#define MAX_ENTITY 100000000 //100 000 000

typedef struct entity_pool {
	// entity_stats
	int32_t		*health;
	uint32_t	*max_health;
	uint16_t 	*regeneration;
	uint16_t	*damage;
	// entity_info
	Vector3		*position;
	Vector3		*min;
	Vector3		*max;
	Vector3		*direction;
	Vector3		*velocity;
	// data control
	uint64_t	controle_bit[MAX_ENTITY / sizeof(uint64_t)];
	uint32_t	size;
	uint32_t	capacity;
}	entity_pool;

typedef struct	entity {
	// entity_stats
	int32_t		health;
	uint32_t	max_health;
	uint16_t 	regeneration;
	uint16_t	damage;
	// entity_info
	Vector3		position;
	Vector3		min;
	Vector3		max;
	Vector3		direction;
	Vector3		velocity;
}	entity;

#endif