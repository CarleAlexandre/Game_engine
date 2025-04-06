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

// ================================================================
// Memory System Core
// ================================================================

static HavenStackAllocator	stack_allocator[2];
static HavenMemoryStats		stats;
static unsigned int		current = 0;

void	haven_memory_system_init() {
	stack_allocator[0].offset = 0;
	stack_allocator[0].size = BASE_STACK_SIZE;
	stack_allocator[0].base = malloc(BASE_STACK_SIZE);
	
	stack_allocator[1].offset = 0;
	stack_allocator[1].size = BASE_STACK_SIZE;
	stack_allocator[1].base = malloc(BASE_STACK_SIZE);
	stats.allocation_count = 2;
	stats.total_allocated = BASE_STACK_SIZE * 2;
	stats.total_freed = 0;
}

void	haven_memory_system_shutdown() {
	free(stack_allocator[0].base);
	free(stack_allocator[1].base);
}

void	haven_memory_system_print() {
	printf("INFO: allocation_count: %i , total_allocated: %.2lld Mb, total_freed: %.2lld Mb\n",
		stats.allocation_count, stats.total_allocated / 1048576, stats.total_freed / 1048576);
}

// ================================================================
// Stack Allocator (Temporary/Fast)
// ================================================================

void	*haven_stack_alloc(size_t size, size_t alignment) {
	uintptr_t raw_addr = (uintptr_t)(stack_allocator[current].base + stack_allocator[current].offset);
	size_t padding = (alignment - (raw_addr % alignment)) % alignment;
	
	if (stack_allocator[current].offset + size + padding > stack_allocator[current].size) return NULL;
	
	void* aligned_ptr = (void*)(raw_addr + padding);
	stack_allocator[current].offset += size + padding;
	return aligned_ptr;
}
    
void	haven_stack_reset(void) {
	stack_allocator[current].offset = 0;
	current ^= 1;
}

// ================================================================
// Pool Allocator (Fixed-Size Objects)
// ================================================================

typedef struct pool_block_s {
    struct pool_block_s *next;
} pool_block_t;

typedef struct {
    pool_block_t *free_list;
    void *memory;
    size_t block_size;
    size_t pool_size;
    size_t alignment;
} pool_allocator_t;

pool_allocator_t *haven_pool_create(size_t block_size, size_t count, size_t alignment) {
    pool_allocator_t *pool = malloc(sizeof(pool_allocator_t));
    
    // Adjust block size for alignment and ensure it can hold a pointer
    size_t aligned_size = ((block_size + sizeof(pool_block_t) - 1) / alignment + 1) * alignment;
    pool->block_size = aligned_size;
    pool->pool_size = aligned_size * count;
    pool->alignment = alignment;
    
    // Allocate aligned memory for the pool
    pool->memory = _aligned_malloc(alignment, pool->pool_size);
    pool->free_list = NULL;
    
    // Initialize the free list
    char *block = (char*)pool->memory;
    for (size_t i = 0; i < count; i++) {
        pool_block_t *node = (pool_block_t*)block;
        node->next = pool->free_list;
        pool->free_list = node;
        block += aligned_size;
    }
    
    return pool;
}

void *haven_pool_alloc(pool_allocator_t *pool) {
    if (!pool->free_list) return NULL;
    
    pool_block_t *block = pool->free_list;
    pool->free_list = block->next;
    return block;
}

void haven_pool_free(pool_allocator_t *pool, void *ptr) {
    if (!ptr || ptr < pool->memory || ptr >= (char*)pool->memory + pool->pool_size) return;
    
    pool_block_t *block = (pool_block_t*)ptr;
    block->next = pool->free_list;
    pool->free_list = block;
}

void haven_pool_destroy(pool_allocator_t *pool) {
    if (pool) {
        free(pool->memory);
        free(pool);
    }
}
