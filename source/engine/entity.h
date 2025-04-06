#ifndef ENTITY_H
# define ENTITY_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <raylib.h>

#define MAX_ENTITY 4096

typedef struct entity_stats {
	int		health;
	uint32_t	max_health;
	unsigned short 	regeneration;
	unsigned short	damage;
}	entity_stats;

typedef struct	entity_info {
	Vector3	position;
	Vector3	min;
	Vector3	max;
	Vector3	direction;
	Vector3	velocity;
}	entity_info;

typedef struct entity_pool {
	entity_stats	*stats;
	entity_info	*info;
}	entity_pool;

typedef struct	entity {
	entity_stats	*stats;
	entity_info	*info;
}	entity;

#endif