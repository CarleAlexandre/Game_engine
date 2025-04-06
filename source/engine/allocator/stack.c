#include "allocator_impl.h"

#define STACK_SIZE 67108864 //64Mb

static allocator_stack	stacks[2];
static uint8_t		current;

void	*allocator_stack_alloc(size_t size, size_t alignment) {
	uintptr_t raw_addr = 0x00;
	void* aligned_ptr = 0x00;
	size_t padding = 0;

	raw_addr = (uintptr_t)(stacks[current].base + stacks[current].offset);
	assert(raw_addr);

	padding = (alignment - (raw_addr % alignment)) % alignment;
	
	if (stacks[current].offset + size + padding > stacks[current].size) {
		return NULL;
	}
	
	aligned_ptr = (void*)(raw_addr + padding);
	stacks[current].offset += size + padding;
	return aligned_ptr;
}
    
void	allocator_stack_swap(void) {
	stacks[current].offset = 0;
	current ^= 1;
}

void	allocator_stack_init(void) {

}

void	allocator_stack_delete(void) {

}
