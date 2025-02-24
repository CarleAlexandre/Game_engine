#include <pthread.h>

// pthread_t *thread_pool;

// void *thread_mgr_function(void *) {
	
// }

// void init_thread_mgr() {
// 	pthread_attr_t	attr;
// 	pthread_t	thread_mgr;
// 	void *(*thread_mgr_func)(void*);
// 	pthread_create( &thread_mgr, &attr, thread_mgr_func, NULL);
// }

// unsigned int	add_task_to_pool(void *(*f)(void *), void *arg) {
// 	pthread_attr_t	attr;
// 	pthread_create(&thread_pool[0], &attr, f, arg);
// 	return (&thread_pool[0]);
// }

// unsigned int	wait_task(unsigned int idx) {
// 	void	*res;
// 	pthread_join(thread_pool[0], &res);
// }

// void	physic_thread() {

// }

// void	meshing_thread() {

// }
