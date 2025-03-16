#include "entity_impl.h"
#include <haven/haven_type.h>
#include <pthread.h>
#include <string.h>

haven_darray_t	*entities_array;
pthread_mutex_t	entities_mtx;

void	init_entities(void) {
	entities_array = init_dyn_array(sizeof(entity_t));
	pthread_mutex_init(&entities_mtx, 0x00);
}

uint32_t	add_entity(entity_t entity) {
	pthread_mutex_lock(&entities_mtx);
	haven_darray_add(entities_array, (char *)&entity);
	pthread_mutex_unlock(&entities_mtx);
	return 0;
}

// should only copy enity not hold pointer for thread safety
entity_t	get_entity(unsigned int idx) {
	entity_t ret;

	pthread_mutex_lock(&entities_mtx);
	entity_t *current = (entity_t *)haven_darray_get(entities_array, idx);
	memmove(&ret, current, sizeof(entity_t));
	pthread_mutex_unlock(&entities_mtx);

	return (ret);
}

void	del_entity(unsigned int idx) {
	pthread_mutex_lock(&entities_mtx);
	haven_darray_del(entities_array, idx);
	pthread_mutex_unlock(&entities_mtx);
}
