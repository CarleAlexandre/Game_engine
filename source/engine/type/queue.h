/*

	dynamic queue isn't done yet do not use

*/


#ifndef QUEUE_H
# define QUEUE_H

#include <stdlib.h>
#include <string.h>
#include <assert.h>

/*
	STATIC QUEUE
*/

#define MAX_QUEUE_SIZE 100

/**
 * @brief 
 * 
 */
typedef struct	static_queue {
	void		*data; // Dynamic byte buffer
	unsigned int	front;
	unsigned int	rear;
	unsigned int	size;
	unsigned int	data_size; // Use larger type (not limited to 255 bytes)
	unsigned int	capacity;
}	static_queue;

/**
 * @brief 
 * 
 * @param data_size 
 * @return static_queue* 
 */
static static_queue*	static_queue_init(unsigned int data_size) {
	static_queue *q;
	q = (static_queue *)malloc(sizeof(static_queue));
	assert(q);
	q->front = 0;
	q->rear = 0;
	q->size = 0;
	q->data_size = data_size;
	q->capacity = MAX_QUEUE_SIZE;
	q->data = (char *)malloc(q->capacity * q->data_size);
	assert(q->data);
	if (!q->data) {
		q->capacity = 0;
	}
}

/**
 * @brief 
 * 
 * @param q 
 * @param value 
 * @return int 
 */
static int	static_queue_push(static_queue *q, const void *value) {
	assert(q);
	if (q->size == q->capacity) {
		return -1; // Queue full
	}
	char *target = q->data + (q->rear * q->data_size);
	memcpy(target, value, q->data_size);
	q->rear = (q->rear + 1) % q->capacity;
	q->size++;
	return 0;
}

/**
 * @brief 
 * 
 * @param q 
 * @param value 
 * @return int 
 */
static int	static_queue_pop(static_queue *q, void *value) {
	assert(q);
	if (q->size == 0) {
		return -1; // Queue empty
	}
	const char *source = q->data + (q->front * q->data_size);
	memcpy(value, source, q->data_size);
	q->front = (q->front + 1) % q->capacity;
	q->size--;
	return 0;
}

/**
 * @brief 
 * 
 * @param q 
 */
static void	static_queue_destroy(static_queue *q) {
	assert(q);
	free(q->data);
	q->data = NULL;
	q->capacity = 0;
	q->size = 0;
}

/*
	DYNAMIC QUEUE
*/

/**
 * @brief 
 * 
 */
typedef struct	dynamic_queue {
	void		*data; // Dynamic byte buffer
	unsigned int	front;
	unsigned int	rear;
	unsigned int	size;
	unsigned int	data_size; // Use larger type (not limited to 255 bytes)
	unsigned int	capacity;
}	dynamic_queue;

/**
 * @brief 
 * 
 * @param q 
 * @param data_size 
 */
static void	dynamic_queue_init(dynamic_queue *q, unsigned int data_size) {
	assert(q);
	q->front = 0;
	q->rear = 0;
	q->size = 0;
	q->data_size = data_size;
	q->capacity = MAX_QUEUE_SIZE;
	q->data = (char *)malloc(q->capacity * q->data_size);
	assert(q->data);
	if (!q->data) {
		// Handle allocation failure (e.g., set capacity to 0)
		q->capacity = 0;
	}
}

/**
 * @brief 
 * 
 * @param q 
 * @param value 
 * @return int 
 */
static int	dynamic_queue_push(dynamic_queue *q, const void *value) {
	assert(q);
	if (q->size == q->capacity) {
		return -1; // Queue full
	}
	char *target = q->data + (q->rear * q->data_size);
	memcpy(target, value, q->data_size);
	q->rear = (q->rear + 1) % q->capacity;
	q->size++;
	return 0;
}

/**
 * @brief 
 * 
 * @param q 
 * @param value 
 * @return int 
 */
static int	dynamic_queue_pop(dynamic_queue *q, void *value) {
	assert(q);
	if (q->size == 0) {
		return -1; // Queue empty
	}
	const char *source = q->data + (q->front * q->data_size);
	memcpy(value, source, q->data_size);
	q->front = (q->front + 1) % q->capacity;
	q->size--;
	return 0;
}

/**
 * @brief 
 * 
 * @param q 
 */
static void	dynamic_queue_destroy(dynamic_queue *q) {
	assert(q);
	free(q->data);
	q->data = NULL;
	q->capacity = 0;
	q->size = 0;
}

#endif