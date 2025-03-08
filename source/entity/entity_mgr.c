#include <entity_impl.h>
#include <data_type/dynamic_array.h>
#include <pthread.h>

dyn_array_t	*info_array;
pthread_mutex_t	info_mtx;

dyn_array_t	*stats_array;
pthread_mutex_t	stats_mtx;

dyn_array_t	*anim_array;
pthread_mutex_t	anim_mtx;

void	init_entities(void) {
	info_array = init_dyn_array(sizeof(entity_info_t));
	stats_array = init_dyn_array(sizeof(entity_stats_t)); 
	anim_array = init_dyn_array(sizeof(entity_animation_t));
	pthread_mutex_init(info_mtx, 0x00);
	pthread_mutex_init(stats_mtx, 0x00);
	pthread_mutex_init(anim_mtx, 0x00);
}

uint32_t	add_entity(void) {
	entity_info_t		init_info = {0};
	entity_stats_t		init_stats = {0};
	entity_animation_t	init_anim = {0};

	pthread_mutex_lock(info_mtx);
	dyn_add_elem(info_array, (char *)&init_info);
	pthread_mutex_unlock(info_mtx);

	pthread_mutex_lock(stats_mtx);
	dyn_add_elem(stats_array, (char *)&init_stats);
	pthread_mutex_unlock(stats_mtx);

	pthread_mutex_lock(anim_mtx);
	dyn_add_elem(anim_array, (char *)&init_anim);
	pthread_mutex_unlock(anim_mtx);
}

// should only copy enity not hold pointer for thread safety
entity_t	get_entity(unsigned int idx) {
	entity_t ret;

	pthread_mutex_lock(info_mtx);
	ret.info = (entity_info_t *)dyn_get(info_array, idx);
	pthread_mutex_unlock(info_mtx);

	pthread_mutex_lock(stats_mtx);
	ret.stats = (entity_stats_t *)dyn_get(stats_array, idx);
	pthread_mutex_unlock(stats_mtx);

	pthread_mutex_lock(anim_mtx);
	ret.anim = (entity_animation_t *)dyn_get(anim_array, idx);
	pthread_mutex_unlock(anim_mtx);

	return (ret);
}

void	set_entity(entity_info_t info, entity_stats_t stats, entity_animation_t anim) {
	
}

void	del_entity(unsigned int idx) {

}