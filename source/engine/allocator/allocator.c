#include "allocator_impl.h"

static memory_stats	stats;

void	memory_system_init() {
	stats = (memory_stats){0};
}

memory_page*	memory_system_alloc(size_t nmemb, size_t size) {
	memory_page* memory;
	memory = malloc(sizeof(memory_page));
	assert(memory);
	memory->size = 1;
	memory->ptr = aligigned_alloc(size, nmemb * size);


	stats.current_allocated += size;
	stats.total_allocated += size;
	stats.total_allocation ++;
	return (memory);
}

void	memory_system_free(memory_page* page) {
	
}

void	memory_system_shutdown() {
	if (stats.current_allocated != 0) {
		printf("WARNING: %.2lld Mb got leaked\n", stats.current_allocated / 1048576);
	}
}

void	memory_system_print() {
	printf("INFO: allocation_count: %i , total_allocated: %.2lld Mb, total_freed: %.2lld Mb, current_allocated: %2lld Mb\n",
		stats.total_allocation, stats.total_allocated / 1048576, stats.total_freed / 1048576, stats.current_allocated / 1048576);
}
