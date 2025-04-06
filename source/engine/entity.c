#include "entity.h"

entity_pool	pool;

void	entity_pool_init(void) {
	pool.info = calloc(MAX_ENTITY, sizeof(entity_info));
	assert(pool.info);
	pool.stats = calloc(MAX_ENTITY, sizeof(entity_stats));
	assert(pool.stats);
}

void	entity_pool_destroy(void) {
	free(pool.info);
	free(pool.stats);
}

void	entity_spawn() {

}

void	entity_clear() {
	
}

void	entity_pool_update() {

}
