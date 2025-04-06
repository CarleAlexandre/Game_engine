#include "allocator_impl.h"


// typedef struct pool_block_s {
//     struct pool_block_s *next;
// } pool_block_t;

// typedef struct {
//     pool_block_t *free_list;
//     void *memory;
//     size_t block_size;
//     size_t pool_size;
//     size_t alignment;
// } pool_allocator_t;

// pool_allocator_t *haven_pool_create(size_t block_size, size_t count, size_t alignment) {
//     pool_allocator_t *pool = malloc(sizeof(pool_allocator_t));
    
//     // Adjust block size for alignment and ensure it can hold a pointer
//     size_t aligned_size = ((block_size + sizeof(pool_block_t) - 1) / alignment + 1) * alignment;
//     pool->block_size = aligned_size;
//     pool->pool_size = aligned_size * count;
//     pool->alignment = alignment;
    
//     // Allocate aligned memory for the pool
//     pool->memory = _aligned_malloc(alignment, pool->pool_size);
//     pool->free_list = NULL;
    
//     // Initialize the free list
//     char *block = (char*)pool->memory;
//     for (size_t i = 0; i < count; i++) {
//         pool_block_t *node = (pool_block_t*)block;
//         node->next = pool->free_list;
//         pool->free_list = node;
//         block += aligned_size;
//     }
    
//     return pool;
// }

// void *haven_pool_alloc(pool_allocator_t *pool) {
//     if (!pool->free_list) return NULL;
    
//     pool_block_t *block = pool->free_list;
//     pool->free_list = block->next;
//     return block;
// }

// void haven_pool_free(pool_allocator_t *pool, void *ptr) {
//     if (!ptr || ptr < pool->memory || ptr >= (char*)pool->memory + pool->pool_size) return;
    
//     pool_block_t *block = (pool_block_t*)ptr;
//     block->next = pool->free_list;
//     pool->free_list = block;
// }

// void haven_pool_destroy(pool_allocator_t *pool) {
//     if (pool) {
//         free(pool->memory);
//         free(pool);
//     }
// }
