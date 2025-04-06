#include <pthread.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "type/queue.h"

#include <time.h>
#include <windows.h>

void	time_usleep(uint64_t usec) {
	HANDLE timer; 
	LARGE_INTEGER ft; 
    
	ft.QuadPart = -(10*usec); // Convert to 100 nanosecond interval, negative value indicates relative time
    
	timer = CreateWaitableTimer(NULL, TRUE, NULL);
	SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0); 
	WaitForSingleObject(timer, INFINITE);
	CloseHandle(timer);
}

typedef struct	s_task {
	void	*(*func)(void *);
	void	*arg;
	int	id;
	bool	completed;
	void	*result;
}	task_t;

static struct {
	static_queue	*async_tasks;
	pthread_mutex_t	async_mutex;
	
	static_queue	*sync_tasks;
	pthread_mutex_t sync_mutex;

	
	bool		running;
	pthread_mutex_t	running_mtx;
	
	int		next_task_id;
	static_queue	*completed_tasks;
	pthread_mutex_t	status_mtx;
	
	pthread_t	workers[32];
}	thread_mgr;

/* 
	worker function
	need to be careful with waiting usleep could be too long or short

*/
static void	*worker_function(void *arg) {
	(void)arg;

	bool run = true;
	while (run) {
		task_t task;
		bool found_task = false;

		pthread_mutex_lock(&thread_mgr.sync_mutex);
		if (static_queue_pop(&thread_mgr.sync_tasks, &task) == 0) {
			found_task = true;
		}
		pthread_mutex_unlock(&thread_mgr.sync_mutex);

		if (!found_task) {
			pthread_mutex_lock(&thread_mgr.async_mutex);
			if (static_queue_pop(&thread_mgr.async_tasks, &task) == 0) {
				found_task = true;
			}
			pthread_mutex_unlock(&thread_mgr.async_mutex);
		}

		if (found_task) {
			task.result = task.func(task.arg);
			
			pthread_mutex_lock(&thread_mgr.status_mtx);
			static_queue_push(&thread_mgr.completed_tasks, &task.id);
			pthread_mutex_unlock(&thread_mgr.status_mtx);
		} else {
			time_usleep(1000);
		}
		pthread_mutex_lock(&thread_mgr.running_mtx);
		run = thread_mgr.running;
		pthread_mutex_unlock(&thread_mgr.running_mtx);
	}
	return (NULL);
}

/*
	add task to pool, data inside arg should be used as input and output
*/
int	thread_task_add(void *(*func)(void *), void *arg, bool is_synced) {
	task_t task = {
		.func = func,
		.arg = arg,
		.completed = false,
		.result = NULL
	};

	pthread_mutex_lock(&thread_mgr.status_mtx);
	task.id = thread_mgr.next_task_id++;
	pthread_mutex_unlock(&thread_mgr.status_mtx);

	if (is_synced) {
		pthread_mutex_lock(&thread_mgr.sync_mutex);
		static_queue_push(&thread_mgr.sync_tasks, &task);
		pthread_mutex_unlock(&thread_mgr.sync_mutex);
	} else {
		pthread_mutex_lock(&thread_mgr.async_mutex);
		static_queue_push(&thread_mgr.async_tasks, &task);
		pthread_mutex_unlock(&thread_mgr.async_mutex);
	}

	return (task.id);
}

//not to rework as i just need to check if first data == id, then pop and return true else return false
bool	thread_task_status(int task_id) {
	pthread_mutex_lock(&thread_mgr.status_mtx);

	int *completed_ids = thread_mgr.completed_tasks->data;
	for (unsigned i = 0; i < thread_mgr.completed_tasks->size; i++) {
		if (completed_ids[i] == task_id) {
			memmove(&completed_ids[i], &completed_ids[i+1], 
				(thread_mgr.completed_tasks->size - i - 1) * sizeof(int));
			thread_mgr.completed_tasks->size--;
			// static_queue_pop(thread_mgr.completed_tasks, task_id);
			pthread_mutex_unlock(&thread_mgr.status_mtx);
			return (true);
		}
	}

	pthread_mutex_unlock(&thread_mgr.status_mtx);
	return (false);
}

/*
	init thread_mgr and worker
*/
void	thread_mgr_init() {
	thread_mgr.async_tasks = static_queue_init(sizeof(task_t));
	thread_mgr.sync_tasks = static_queue_init(sizeof(task_t));
	thread_mgr.completed_tasks = static_queue_init(sizeof(int));

	pthread_mutex_init(&thread_mgr.async_mutex, NULL);
	pthread_mutex_init(&thread_mgr.sync_mutex, NULL);
	pthread_mutex_init(&thread_mgr.status_mtx, NULL);
	pthread_mutex_init(&thread_mgr.running_mtx, NULL);

	thread_mgr.running = true;
	thread_mgr.next_task_id = 1;

	for (int i = 0; i < 32; i++) {
		pthread_create(&thread_mgr.workers[i], NULL, worker_function, NULL);
	}
}

/*
	raise stopping flag to all thread then wait for them to close
*/
void	thread_mgr_close() {
	pthread_mutex_lock(&thread_mgr.running_mtx);
	thread_mgr.running = false;
	pthread_mutex_unlock(&thread_mgr.running_mtx);

	for (int i = 0; i < 32; i++) {
		pthread_join(thread_mgr.workers[i], NULL);
	}

	static_queue_destroy(&thread_mgr.async_tasks);
	static_queue_destroy(&thread_mgr.sync_tasks);
	static_queue_destroy(&thread_mgr.completed_tasks);

	pthread_mutex_destroy(&thread_mgr.async_mutex);
	pthread_mutex_destroy(&thread_mgr.sync_mutex);
	pthread_mutex_destroy(&thread_mgr.status_mtx);
	pthread_mutex_destroy(&thread_mgr.running_mtx);
}
