#include "alloc_impl.h"

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
	printf("allocation_count: %i , total_allocated: %.2f Mb, total_freed: %.2f Mb",
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
/*
HavenPoolAllocator*	haven_pool_create(size_t object_size, size_t alignment, uint32_t capacity) {
	HavenPoolAllocator *pool;
	
	pool = malloc(sizeof(HavenPoolAllocator));
	
	if (pool) {
		pool->alignment = alignment;
		pool->capacity = capacity;
		pool->object_size = object_size;
		
		pool->free_list = 0x00;
		pool->memory = 0x00;
	}
	return (pool);
}

void	haven_pool_destroy(HavenPoolAllocator* allocator) {
	
}

void*	haven_pool_alloc(HavenPoolAllocator* allocator) {
	
}

void	haven_pool_free(HavenPoolAllocator* allocator, void* ptr) {
	
}
*/
