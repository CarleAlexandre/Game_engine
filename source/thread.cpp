// #include <pthread.h>
// #include <queue>

// std::queue<pthread_t> thread_pool;

// void *thread_mgr_function(void *) {
	
// }

// void init_thread_mgr() {
// 	pthread_attr_t attr;
// 	pthread_t thread_mgr;
// 	void *(*thread_mgr_func)(void*);
// 	pthread_create( &thread_mgr, &attr, thread_mgr_func, NULL);
// }

// unsigned int add_task_to_pool(void *(*f)(void *), void *arg) {
// 	pthread_t thread;
// 	pthread_attr_t attr;
// 	pthread_create(&thread, &attr, f, arg);
// 	thread_pool.push(thread);
// }

// bool is_task_ready(unsigned int task_id) {

// }
