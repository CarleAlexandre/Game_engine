#include <entity_impl.h>
#include <data_type/dynamic_array.h>
#include <pthread.h>

dyn_array_t	*entities_array;
pthread_mutex_t	entities_mtx;

void	init_entities(void) {
	entities_array = init_dyn_array(sizeof(entity_t));
	pthread_mutex_init(entities_mtx, 0x00);
}

uint32_t	add_entity(entity_t entity) {
	pthread_mutex_lock(entities_mtx);
	dyn_add_elem(entities_array, (char *)&entity);
	pthread_mutex_unlock(entities_mtx);
}

// should only copy enity not hold pointer for thread safety
entity_t	get_entity(unsigned int idx) {
	entity_t ret;

	pthread_mutex_lock(entities_mtx);
	entity_t *current = (entity_t *)dyn_get(entities_array, idx);
	memmove(&ret, current, sizeof(entity_t));
	pthread_mutex_unlock(entities_mtx);

	return (ret);
}

void	del_entity(unsigned int idx) {
	pthread_mutex_lock(entities_mtx);
	dyn_del_elem(entities_array, idx);
	pthread_mutex_unlock(entities_mtx);
}