#include "allocator_impl.h"

static memory_stats	stats;

void	memory_system_init() {
	stats = (memory_stats){0};
}

void*	memory_system_alloc(size_t allocated) {

}

void	memory_system_free(page) {

}

void	memory_system_shutdown() {
	if (stats.current_allocated != 0) {
		printf("WARNING: %.2lld Mb got leaked\n", stats.current_allocated / 1048576);
	}
}

void	memory_system_print() {

	// printf("INFO: allocation_count: %i , total_allocated: %.2lld Mb, total_freed: %.2lld Mb\n",
		// stats.allocation_count, stats.total_allocated / 1048576, stats.total_freed / 1048576);
}
