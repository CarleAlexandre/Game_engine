#include <stdlib.h>

/*
	STATIC QUEUE (squeue)
*/

#define MAX_QUEUE_SIZE 100

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
void	haven_squeue_init(haven_squeue_t *q, unsigned int data_size) {
	q->front = 0;
	q->rear = 0;
	q->size = 0;
	q->data_size = data_size;
	q->capacity = MAX_QUEUE_SIZE;
	q->data = (char *)malloc(q->capacity * q->data_size);
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
int	haven_squeue_push(haven_squeue_t *q, const void *value) {
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
int	haven_squeue_pop(haven_squeue_t *q, void *value) {
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
void	haven_squeue_destroy(haven_squeue_t *q) {
	free(q->data);
	q->data = NULL;
	q->capacity = 0;
	q->size = 0;
}

/*
	DYNAMIC QUUEUE (dqueue)
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
void	haven_dqueue_init(haven_dqueue_t *q, unsigned int data_size) {
	q->front = 0;
	q->rear = 0;
	q->size = 0;
	q->data_size = data_size;
	q->capacity = MAX_QUEUE_SIZE;
	q->data = (char *)malloc(q->capacity * q->data_size);
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
int	haven_dqueue_push(haven_dqueue_t *q, const void *value) {
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
int	haven_dqueue_pop(haven_dqueue_t *q, void *value) {
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
void	haven_dqueue_destroy(haven_dqueue_t *q) {
	free(q->data);
	q->data = NULL;
	q->capacity = 0;
	q->size = 0;
}
