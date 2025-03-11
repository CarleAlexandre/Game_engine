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
char	*haven_file_read(const char* filename);

/**
 * @brief 
 * 
 * @param data 
 * @param data_length 
 * @param filename 
 * @param append 
 */
void	haven_file_write(const char *data, size_t data_length, const char *filename, bool append);

/**
 * @brief 
 * 
 * @param stat 
 * @param desc 
 * @param line 
 * @return int 
 */
int	haven_cmd_log(const char *stat, const char *desc, const int line);

#endif