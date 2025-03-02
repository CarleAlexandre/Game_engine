#ifndef CORE_H
# define CORE_H

#include <stdbool.h>
#include <stdint.h>

/*

*/
void	init_thread_mgr();

/*

*/
void	close_thread_mgr();

/*

*/
int	check_task_status(int task_id);

/*

*/
int	add_task_to_pool(void *(*func)(void *), void *arg, bool is_synced);

/*

*/
void	usleep(__int64 usec);

#endif
