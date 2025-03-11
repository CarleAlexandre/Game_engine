#ifndef ALLOC_IMPL_H
# define ALLOC_IMPL_H

#include <stdint.h>
#include <stdlib.h>

typedef struct {
	size_t	total_allocated;
	size_t	total_freed;
	uint32_t	allocation_count;
}	HavenMemoryStats;

#define BASE_STACK_SIZE 67108864 //byte, equal to 64Mb

typedef struct {
	uint8_t	*base;
	size_t	size;
	size_t	offset;
}	HavenStackAllocator;

#endif