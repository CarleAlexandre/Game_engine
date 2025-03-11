#include "alloc_impl.h"

HavenMemoryStats	haven_memory_system_stats() {
}

HavenStackAllocator	stack_allocator[2];
unsigned int		current = 0;

void	haven_memory_system_init() {
	stack_allocator[0].offset = 0;
	stack_allocator[0].size = BASE_STACK_SIZE;
	stack_allocator[0].base = malloc(BASE_STACK_SIZE);

	stack_allocator[1].offset = 0;
	stack_allocator[1].size = BASE_STACK_SIZE;
	stack_allocator[1].base = malloc(BASE_STACK_SIZE);
}

void	haven_memory_system_shutdown() {
	free(stack_allocator[0].base);
	free(stack_allocator[1].base);
}

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
