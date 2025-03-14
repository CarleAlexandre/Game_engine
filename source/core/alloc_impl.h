#ifndef ALLOC_IMPL_H
# define ALLOC_IMPL_H

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
	size_t		total_allocated;
	size_t		total_freed;
	uint32_t	allocation_count;
}	HavenMemoryStats;

#define BASE_STACK_SIZE 67108864 //byte, equal to 64Mb

typedef struct {
	size_t		size;
	size_t		offset;
	uint8_t*	base;
}	HavenStackAllocator;

typedef struct {
	uint8_t*	memory;
	size_t		object_size;
	size_t		alignment;
	uint32_t	capacity;
	void*		free_list;
}	HavenPoolAllocator;    

typedef struct {
	uint8_t* memory;
	size_t total_size;
	void* head;
}	HavenFreeListAllocator;

#endif