#ifndef HAVEN_TIME_H
# define HAVEN_TIME_H

#include <stdint.h>

/**
 * @brief The usleep() function suspends execution of the calling thread for
       (at least) usec microseconds.  The sleep may be lengthened
       slightly by any system activity or by the time spent processing
       the call or by the granularity of system timers.
 * 
 * @param usec
 */
void	haven_time_usleep(uint64_t usec);

/**
 * @brief initialize time
 * 
 */
void	haven_time_init();

/**
 * @brief update frame time
 * 
 */
void	haven_time_update();

/**
 * @brief get frame time
 * 
 * @return double 
 */
double	haven_time_frame();

/**
 * @brief get time elapsed since haven_time_init
 * 
 * @return double
 */
double	haven_time_elapsed();

#endif
