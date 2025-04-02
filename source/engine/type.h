#ifndef HAVEN_TYPE_H
# define HAVEN_TYPE_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/**
 * @brief static queue data structure
 * 
 */
typedef struct	haven_squeue_s {
	void		*data; // Dynamic byte buffer
	unsigned int	front;
	unsigned int	rear;
	unsigned int	size;
	unsigned int	data_size; // Use larger type (not limited to 255 bytes)
	unsigned int	capacity;
}	haven_squeue_t;

/**
 * @brief initialize a static queue (max 100 element of data_size per queue)
 * 
 * @param q 
 * @param data_size 
 */
void	haven_squeue_init(haven_squeue_t *q, unsigned int data_size);

/**
 * @brief 
 * 
 * @param q 
 * @param value 
 * @return int 
 */
int	haven_squeue_push(haven_squeue_t *q, const void *value);

/**
 * @brief 
 * 
 * @param q 
 * @param value 
 * @return int 
 */
int	haven_squeue_pop(haven_squeue_t *q, void *value);

/**
 * @brief 
 * 
 * @param q 
 */
void	haven_squeue_destroy(haven_squeue_t *q);

/*
	DYNAMIC QUUEUE (dqueue) note implemented yet
*/

/**
 * @brief dynamic queue data structure
 * 
 */
typedef struct	haven_dqueue_s {
	void		*data; // Dynamic byte buffer
	unsigned int	front;
	unsigned int	rear;
	unsigned int	size;
	unsigned int	data_size; // Use larger type (not limited to 255 bytes)
	unsigned int	capacity;
}	haven_dqueue_t;

/**
 * @brief initialize a dynamic queue
 * 
 * @param q 
 * @param data_size 
 */
void	haven_dqueue_init(haven_dqueue_t *q, unsigned int data_size);

/**
 * @brief 
 * 
 * @param q 
 * @param value 
 * @return int 
 */
int	haven_dqueue_push(haven_dqueue_t *q, const void *value);

/**
 * @brief 
 * 
 * @param q 
 * @param value 
 * @return int 
 */
int	haven_dqueue_pop(haven_dqueue_t *q, void *value);

/**
 * @brief 
 * 
 * @param q 
 */
void	haven_dqueue_destroy(haven_dqueue_t *q);

/*

*/

/**
 * @brief dynamic array data structure
 * 
 */
typedef struct dynamic_array {
	void*		data;
	unsigned int	size;// Number of elements in the array
	unsigned int	capacity;// Total capacity of the array (in bytes)
	unsigned char	data_size;// Size of each element (in bytes)
}	dynamic_array;

/**
 * @brief 
 * 
 * @param array 
 * @param element 
 */
void	dynamic_array_add(dynamic_array *array, void *element);

/**
 * @brief 
 * 
 * @param array 
 * @param idx 
 */
void	dynamic_array_del(dynamic_array *array, unsigned int idx);

/**
 * @brief 
 * 
 * @param array 
 */
void	dynamic_array_clear(dynamic_array *array);

/**
 * @brief 
 * 
 * @param data_size 
 * @return haven_darray_t* 
 */
dynamic_array*	dynamic_array_init(unsigned char data_size);

/**
 * @brief 
 * 
 * @param array 
 */
void	dynamic_array_destroy(dynamic_array* array);

/**
 * @brief 
 * 
 * @param array 
 * @param start 
 * @param end 
 */
void	haven_darray_range_remove(dynamic_array* array, unsigned int start, unsigned int end);

/*
 * @brief 
 * 
 * @param array 
 * @param src_start 
 * @param src_end 
 * @param dst 
 */
//void	haven_darray_range_move(haven_darray_t* array, unsigned int src_start, unsigned int src_end, unsigned int dst);

/**
 * @brief 
 * 
 * @param array 
 * @param comparator 
 */
void	haven_darray_sort(dynamic_array *array, int (*comparator)(const void *, const void *));

/**
 * @brief 
 * 
 * @param array 
 * @param idx 
 * @return void* 
 */
void*	haven_darray_get(dynamic_array *array,unsigned int idx);

#endif