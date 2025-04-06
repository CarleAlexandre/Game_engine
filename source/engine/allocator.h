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

/**
 * @brief 
 * 
 */
void	haven_memory_system_init();

/**
 * @brief 
 * 
 */
void	haven_memory_system_shutdown();

/**
 * @brief 
 * 
 */
void	haven_memory_system_print();

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
// Pool Allocator (Fixed-Size Objects) WIP
// ================================================================

/**
 * @brief 
 * 
 */
typedef struct {
	size_t		object_size;
	size_t		alignment;
	uint8_t*	memory;
	void*		free_list;
	uint32_t	capacity;
} HavenPoolAllocator;

/**
 * @brief create a pool object, with initial capacity
 * 
 * @param object_size 
 * @param alignment 
 * @param capacity 
 * @return HavenPoolAllocator* 
 */
HavenPoolAllocator*	haven_pool_create(size_t object_size, size_t alignment, uint32_t capacity);

/**
 * @brief 
 * 
 * @param allocator 
 */
void	haven_pool_destroy(HavenPoolAllocator* allocator);

/**
 * @brief 
 * 
 * @param allocator 
 * @return void* 
 */
void*	haven_pool_alloc(HavenPoolAllocator* allocator);

/**
 * @brief 
 * 
 * @param allocator 
 * @param ptr 
 */
void	haven_pool_free(HavenPoolAllocator* allocator, void* ptr);

// ================================================================
// Free-List Allocator (Dynamic-Sized) WIP
// ================================================================
typedef struct {
    uint8_t* memory;
    size_t total_size;
    void* head;
} HavenFreeListAllocator;

/**
 * @brief 
 * 
 * @param size 
 * @return HavenFreeListAllocator* 
 */
HavenFreeListAllocator* haven_freelist_create(size_t size);
void haven_freelist_destroy(HavenFreeListAllocator* allocator);

void* haven_freelist_alloc(HavenFreeListAllocator* allocator, size_t size, size_t alignment);
void haven_freelist_free(HavenFreeListAllocator* allocator, void* ptr);

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