
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include "haven/haven_type.h"

#define MAX_CHUNK_CACHE 1024
#define RLE_MARKER 0xFF

typedef struct {
    unsigned char *data;
    size_t compressed_size;
    size_t raw_size;
    time_t last_update;
    int chunk_x;
    int chunk_y; 
    int chunk_z;
    haven_octree_t *octree;
} ChunkData;

typedef struct {
    ChunkData *chunks;
    size_t count;
    size_t capacity;
} ChunkCache;

// Convert grid data to haven_octree
haven_octree_t *grid_to_octree(const unsigned char *grid, int size, int x, int y, int z, int current_size) {
    haven_octree_node_t *node = haven_octree_node_create();
    if (!node) return NULL;

    // Base case: single voxel or uniform region
    if (current_size == 1) {
        node->value = grid[x + y * size + z * size * size];
        node->is_leaf = true;
        return node;
    }

    bool uniform = true;
    unsigned char first_value = grid[x + y * size + z * size * size];
    
    // Check if region is uniform
    int half = current_size / 2;
    for (int dy = 0; dy < current_size && uniform; dy++) {
        for (int dx = 0; dx < current_size && uniform; dx++) {
            for (int dz = 0; dz < current_size && uniform; dz++) {
                if (grid[(x + dx) + (y + dy) * size + (z + dz) * size * size] != first_value) {
                    uniform = false;
                }
            }
        }
    }

    if (uniform) {
        node->value = first_value;
        node->is_leaf = true;
        return node;
    }

    // Subdivide non-uniform regions
    node->is_leaf = false;
    for (int i = 0; i < 8; i++) {
        int new_x = x + (i & 4 ? half : 0);
        int new_y = y + (i & 2 ? half : 0);
        int new_z = z + (i & 1 ? half : 0);
        node->children[i] = grid_to_octree(grid, size, new_x, new_y, new_z, half);
    }

    return node;
}

// Convert haven_octree to grid data
void octree_to_grid(haven_octree_node_t *node, unsigned char *grid, int size, int x, int y, int z, int current_size) {
    if (!node || !grid) return;

    if (node->is_leaf) {
        // Fill the entire region with the node's value
        for (int dy = 0; dy < current_size; dy++) {
            for (int dx = 0; dx < current_size; dx++) {
                for (int dz = 0; dz < current_size; dz++) {
                    grid[(x + dx) + (y + dy) * size + (z + dz) * size * size] = node->value;
                }
            }
        }
        return;
    }

    // Recursively process children
    int half = current_size / 2;
    for (int i = 0; i < 8; i++) {
        if (node->children[i]) {
            int new_x = x + (i & 4 ? half : 0);
            int new_y = y + (i & 2 ? half : 0);
            int new_z = z + (i & 1 ? half : 0);
            octree_to_grid(node->children[i], grid, size, new_x, new_y, new_z, half);
        }
    }
}

// Compression using Run Length Encoding
size_t compress_chunk_data(const unsigned char *input, size_t input_size, 
                          unsigned char *output, size_t output_capacity) {
    if (!input || !output || output_capacity < 2) return 0;
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input_size) {
        unsigned char current = input[in_pos];
        size_t run_length = 1;
        
        while (in_pos + run_length < input_size && 
               input[in_pos + run_length] == current && 
               run_length < 254) {
            run_length++;
        }
        
        if (out_pos + 3 > output_capacity) return 0;
        
        if (run_length > 3 || current == RLE_MARKER) {
            output[out_pos++] = RLE_MARKER;
            output[out_pos++] = run_length;
            output[out_pos++] = current;
        } else {
            while (run_length--) {
                output[out_pos++] = current;
            }
        }
        
        in_pos += run_length;
    }
    
    return out_pos;
}

size_t decompress_chunk_data(const unsigned char *input, size_t input_size,
                            unsigned char *output, size_t output_capacity) {
    if (!input || !output) return 0;
    
    size_t out_pos = 0;
    size_t in_pos = 0;
    
    while (in_pos < input_size) {
        if (input[in_pos] == RLE_MARKER) {
            if (in_pos + 2 >= input_size) return 0;
            
            size_t run_length = input[in_pos + 1];
            unsigned char value = input[in_pos + 2];
            
            if (out_pos + run_length > output_capacity) return 0;
            
            memset(output + out_pos, value, run_length);
            out_pos += run_length;
            in_pos += 3;
        } else {
            if (out_pos >= output_capacity) return 0;
            output[out_pos++] = input[in_pos++];
        }
    }
    
    return out_pos;
}

ChunkCache *create_chunk_cache(void) {
    ChunkCache *cache = malloc(sizeof(ChunkCache));
    if (!cache) return NULL;
    
    cache->chunks = malloc(sizeof(ChunkData) * MAX_CHUNK_CACHE);
    if (!cache->chunks) {
        free(cache);
        return NULL;
    }
    
    cache->count = 0;
    cache->capacity = MAX_CHUNK_CACHE;
    return cache;
}

void destroy_chunk_cache(ChunkCache *cache) {
    if (!cache) return;
    
    for (size_t i = 0; i < cache->count; i++) {
        free(cache->chunks[i].data);
        haven_octree_destroy(cache->chunks[i].octree);
    }
    free(cache->chunks);
    free(cache);
}

static void shift_chunks_right(ChunkCache *cache, size_t from) {
    if (from >= cache->count) return;
    
    if (cache->count == cache->capacity) {
        free(cache->chunks[cache->capacity - 1].data);
        haven_octree_destroy(cache->chunks[cache->capacity - 1].octree);
        cache->count--;
    }
    
    memmove(&cache->chunks[from + 1], 
            &cache->chunks[from], 
            (cache->count - from) * sizeof(ChunkData));
}

int stream_chunk_data(ChunkCache *cache, 
                     const unsigned char *data, 
                     size_t raw_size,
                     int x, int y, int z,
                     int chunk_size) {
    if (!cache || !data) return -1;
    
    size_t compress_buffer_size = raw_size + (raw_size / 254) * 2 + 1;
    unsigned char *compressed = malloc(compress_buffer_size);
    if (!compressed) return -1;
    
    size_t compressed_size = compress_chunk_data(data, raw_size, 
                                               compressed, compress_buffer_size);
    if (compressed_size == 0) {
        free(compressed);
        return -1;
    }
    
    ChunkData new_chunk = {
        .data = malloc(compressed_size),
        .compressed_size = compressed_size,
        .raw_size = raw_size,
        .last_update = time(NULL),
        .chunk_x = x,
        .chunk_y = y,
        .chunk_z = z,
        .octree = grid_to_octree(data, chunk_size, 0, 0, 0, chunk_size)
    };
    
    if (!new_chunk.data || !new_chunk.octree) {
        free(compressed);
        free(new_chunk.data);
        haven_octree_destroy(new_chunk.octree);
        return -1;
    }
    
    memcpy(new_chunk.data, compressed, compressed_size);
    free(compressed);
    
    size_t insert_pos = 0;
    while (insert_pos < cache->count && 
           cache->chunks[insert_pos].last_update > new_chunk.last_update) {
        insert_pos++;
    }
    
    shift_chunks_right(cache, insert_pos);
    cache->chunks[insert_pos] = new_chunk;
    cache->count++;
    
    return 0;
}

unsigned char *get_chunk_data(ChunkCache *cache, int x, int y, int z, 
                            size_t *out_size) {
    if (!cache || !out_size) return NULL;
    
    for (size_t i = 0; i < cache->count; i++) {
        if (cache->chunks[i].chunk_x == x &&
            cache->chunks[i].chunk_y == y &&
            cache->chunks[i].chunk_z == z) {
            
            unsigned char *decompressed = malloc(cache->chunks[i].raw_size);
            if (!decompressed) return NULL;
            
            size_t decompressed_size = decompress_chunk_data(
                cache->chunks[i].data, 
                cache->chunks[i].compressed_size,
                decompressed, 
                cache->chunks[i].raw_size
            );
            
            if (decompressed_size == 0) {
                free(decompressed);
                return NULL;
            }
            
            *out_size = decompressed_size;
            return decompressed;
        }
    }
    
    return NULL;
}



