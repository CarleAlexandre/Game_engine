#include "entity.h"

entity_pool	pool;

void	entity_pool_init(void) {
	// entity_stats
	pool.health = calloc(MAX_ENTITY, sizeof(int32_t));
	pool.max_health = calloc(MAX_ENTITY, sizeof(uint32_t));
	pool.regeneration = calloc(MAX_ENTITY, sizeof(uint16_t));
	pool.damage = calloc(MAX_ENTITY, sizeof(uint16_t));
	// entity_info
	pool.position = calloc(MAX_ENTITY, sizeof(Vector3));
	pool.min = calloc(MAX_ENTITY, sizeof(Vector3));
	pool.max = calloc(MAX_ENTITY, sizeof(Vector3));
	pool.direction = calloc(MAX_ENTITY, sizeof(Vector3));
	pool.velocity = calloc(MAX_ENTITY, sizeof(Vector3));

	assert(pool.health);
	assert(pool.max_health);
	assert(pool.regeneration);
	assert(pool.damage);
	assert(pool.position);
	assert(pool.min);
	assert(pool.max);
	assert(pool.direction);
	assert(pool.velocity);

	pool.capacity = MAX_ENTITY;
	pool.size = 0;
	memset(pool.controle_bit, 0, sizeof(pool.controle_bit));
}

void	entity_pool_destroy(void) {
	// entity_stats
	free(pool.health);
	free(pool.max_health);
	free(pool.regeneration);
	free(pool.damage);
	// entity_info
	free(pool.position);
	free(pool.min);
	free(pool.max);
	free(pool.direction);
	free(pool.velocity);
}

void	entity_pool_update() {
}

void	entity_spawn(entity nwaps) {
	
}

void	entity_kill() {
	
}
