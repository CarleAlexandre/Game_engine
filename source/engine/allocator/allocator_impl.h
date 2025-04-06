#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include <string.h>

typedef struct {
	size_t		total_allocated;
	size_t		total_allocated;
	size_t		total_freed;
	uint32_t	total_allocation;
	size_t		current_allocated;
}	memory_stats;

#define BASE_STACK_SIZE 67108864 //64Mb

typedef struct {
	size_t		size;
	size_t		offset;
	uint8_t*	base;
}	allocator_stack;
