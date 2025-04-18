#ifndef HAVEN_IO_H
# define HAVEN_IO_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @brief 
 * 
 * @param filename 
 * @return char* 
 */
char	*file_read(const char* filename);

/**
 * @brief 
 * 
 * @param data 
 * @param data_length 
 * @param filename 
 * @param append 
 */
void	file_write(const char *data, uint64_t data_length, const char *filename, bool append);

/**
 * @brief 
 * 
 * @param stat 
 * @param desc 
 * @param line 
 * @return int 
 */
int	cmd_log(const char *stat, const char *desc, const int line);

#endif