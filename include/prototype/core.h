#ifndef CORE_H
# define CORE_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @brief Initialize thread manager structure and start worker threads
 * 
 */
void	init_thread_mgr();

/**
 * @brief Send stop signale to evey worker threads and clear task queue
 * 
 */
void	close_thread_mgr();

/**
 * @brief Check task status by sending task id
 * 
 * @param task_id 
 * @return int 
 */
bool	check_task_status(int task_id);

/**
 * @brief Add a task to the task pool synced or not and return task id as int
 * 
 * @param func 
 * @param arg 
 * @param is_synced 
 * @return int
 */
int	add_task_to_pool(void *(*func)(void *), void *arg, bool is_synced);

/**
 * @brief suspend execution for microsecond intervals
 * 
 * The usleep() function suspends execution of the calling thread for
       (at least) usec microseconds.  The sleep may be lengthened
       slightly by any system activity or by the time spent processing
       the call or by the granularity of system timers.
 * 
 * @param usec 
 */
void	usleep(__int64 usec);



#endif
