#ifndef HAVEN_ALLOC_H
# define HAVEN_ALLOC_H

#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

// ================================================================
// Memory System Core
// ================================================================


typedef struct {
	size_t	total_allocated;
	size_t	total_freed;
	uint32_t	allocation_count;
}	HavenMemoryStats;

void	haven_memory_system_init();
void	haven_memory_system_shutdown();
HavenMemoryStats	haven_memory_system_stats();

// ================================================================
// Stack Allocator (Temporary/Fast)
// ================================================================

/**
 * @brief return a pointer to position x in stack buffer
 * 
 * @param size 
 * @param alignment 
 * @return void* 
 */
void	*haven_stack_alloc(size_t size, size_t alignment);

/**
 * @brief call this function to reset memory offset
 * and  swap stack memory buffer (in this order)
 * 
 */
void	haven_stack_reset(void);

// ================================================================
// Pool Allocator (Fixed-Size Objects)
// ================================================================
typedef struct {
    uint8_t* memory;
    size_t object_size;
    size_t alignment;
    uint32_t capacity;
    void* free_list;
} HavenPoolAllocator;

HavenPoolAllocator* haven_pool_create(size_t object_size, size_t alignment, uint32_t capacity);
void* haven_pool_alloc(HavenPoolAllocator* allocator);
void haven_pool_free(HavenPoolAllocator* allocator, void* ptr);
void haven_pool_destroy(HavenPoolAllocator* allocator);

// ================================================================
// Free-List Allocator (Dynamic-Sized)
// ================================================================
typedef struct {
    uint8_t* memory;
    size_t total_size;
    void* head;
} HavenFreeListAllocator;

HavenFreeListAllocator* haven_freelist_create(size_t size);
void* haven_freelist_alloc(HavenFreeListAllocator* allocator, size_t size, size_t alignment);
void haven_freelist_free(HavenFreeListAllocator* allocator, void* ptr);
void haven_freelist_destroy(HavenFreeListAllocator* allocator);

// ================================================================
// Vulkan/OpenGL Aware Allocators
// ================================================================
/*
typedef struct {
	void* mapped_ptr;     // For persistent mapping
	VkBuffer buffer;      // Vulkan buffer handle
	VkDeviceMemory memory;// Vulkan memory handle
	size_t size;
} HavenGPUMemoryBlock;

HavenGPUMemoryBlock haven_gpu_alloc_buffer(
	size_t size,
	VkBufferUsageFlags usage,
	VkMemoryPropertyFlags memory_flags
);
void haven_gpu_free_buffer(HavenGPUMemoryBlock* block);
*/

#endif