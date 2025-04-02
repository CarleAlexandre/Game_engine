#ifndef HAVEN_THREAD_H
# define HAVEN_THREAD_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief The usleep() function suspends execution of the calling thread for
       (at least) usec microseconds.  The sleep may be lengthened
       slightly by any system activity or by the time spent processing
       the call or by the granularity of system timers.
 * 
 * @param usec
 */
void	time_usleep(uint64_t usec);

/**
 * @brief Initialize thread manager structure and start worker threads
 * 
 */
void	thread_mgr_init();

/**
 * @brief Send stop signale to evey worker threads and clear task queue
 * 
 */
void	thread_mgr_close();

/**
 * @brief Check task status by sending task id
 * 
 * @param task_id 
 * @return int 
 */
bool	thread_task_status(int task_id);

/**
 * @brief Add a task to the task pool synced or not and return task id as int
 * 
 * @param func 
 * @param arg 
 * @param is_synced 
 * @return int
 */
int	thread_task_add(void *(*func)(void *), void *arg, bool is_synced);

#endif
