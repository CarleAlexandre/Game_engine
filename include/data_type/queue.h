#ifndef QUEUE_H
# define QUEUE_H

#include <stdlib.h>

#define MAX_QUEUE_SIZE 100

typedef struct	s_queue {
	void		*data; // Dynamic byte buffer
	unsigned int	front;
	unsigned int	rear;
	unsigned int	size;
	unsigned int	data_size; // Use larger type (not limited to 255 bytes)
	unsigned int	capacity;
}	queue_t;

void init_queue(queue_t *q, unsigned int data_size) {
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

//push new value at end of queue
int push_queue(queue_t *q, const void *value) {
	if (q->size == q->capacity) {
		return -1; // Queue full
	}
	char *target = q->data + (q->rear * q->data_size);
	memcpy(target, value, q->data_size);
	q->rear = (q->rear + 1) % q->capacity;
	q->size++;
	return 0;
}

//pop value at start of queue
int pop_queue(queue_t *q, void *value) {
	if (q->size == 0) {
		return -1; // Queue empty
	}
	const char *source = q->data + (q->front * q->data_size);
	memcpy(value, source, q->data_size);
	q->front = (q->front + 1) % q->capacity;
	q->size--;
	return 0;
}

void cleanup_queue(queue_t *q) {
	free(q->data);
	q->data = NULL;
	q->capacity = 0;
	q->size = 0;
}

#endif
