#include <pthread.h>
#include <stddef.h>
#include <stdint.h>
#include "queue.h"

queue_t		task_pool;
pthread_mutex_t	task_pool_mtx;

queue_t		sync_task_pool;
pthread_mutex_t	sync_task_pool_mtx;

char		thrd_mgr = 1;
pthread_mutex_t thread_mgr_mtx;

void *thread_mgr_function(void *) {
	pthread_t	thread_pool[32];
	char		run = 1;
	
	while (run) {
		pthread_mutex_lock(thread_mgr_mtx);
		run = thrd_mgr;
		pthread_mutex_unlock(thread_mgr_mtx);

		
	}
}


int	add_task_to_pool(void *(*f)(void *), void *arg, char is_synced) {
	static int idx = 1;

	if (is_synced) {
		(idx++ % _I32_MAX);
		pthread_mutex_lock(sync_task_pool_mtx);
		push_queue(&sync_task_pool, 1);
		pthread_mutex_unlock(sync_task_pool_mtx);
		return (idx);
	}
	pthread_mutex_lock(task_pool_mtx);
	push_queue(&task_pool, 1);
	pthread_mutex_unlock(task_pool_mtx);
	return (-1);
}

int	check_task_status(unsigned int idx) {
	
	return (0);
}

void	init_thread_mgr() {
	pthread_t	thread_mgr;

	pthread_create(&thread_mgr, NULL, thread_mgr_function, NULL);
	pthread_mutex_init(&task_pool_mtx, 0x00);
	pthread_mutex_init(&thread_mgr_mtx, 0x00);
	pthread_detach(thread_mgr);
}

void	close_thread_mgr() {
	pthread_mutex_lock(thread_mgr_mtx);
	thrd_mgr = 0;
	pthread_mutex_unlock(thread_mgr_mtx);
}
