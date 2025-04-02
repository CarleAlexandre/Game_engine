// #include <stdbool.h>
// #include <stdint.h>
// #include <stddef.h>
// #include <stdlib.h>
// #include <cglm/cglm.h>
// #include <math.h>
// #include <string.h>

// #define CHUNK_SIZE 16  // Size of each chunk in voxel units
// #define MAX_CHUNKS 4096  // Maximum number of chunks in the world
// #define MEMORY_POOL_SIZE 65536  // Size of memory pool for octree nodes

// // Pre-calculated lookup table for child indices based on point position
// static const uint8_t CHILD_INDEX_TABLE[8][3] = {
//     {0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {1, 1, 0},
//     {0, 0, 1}, {1, 0, 1}, {0, 1, 1}, {1, 1, 1}
// };

// typedef struct	haven_octree_node_s {
// 	bool				isleaf;
// 	void				*data;
// 	struct haven_octree_node_s	*children[8];
// }	haven_octree_node_t;

// // Memory pool for octree nodes
// typedef struct haven_memory_pool_s {
//     haven_octree_node_t nodes[MEMORY_POOL_SIZE];
//     bool used[MEMORY_POOL_SIZE];
//     int next_free;
//     int count;
// } haven_memory_pool_t;

// // Chunk structure to divide the world
// typedef struct haven_chunk_s {
//     int x, y, z;  // Chunk coordinates
//     bool active;  // Whether this chunk is loaded
//     bool modified;  // Whether this chunk has been modified since last save
//     haven_octree_node_t *root;  // Octree for this chunk
// } haven_chunk_t;

// typedef	struct	haven_octree_s {
// 	int			size;  // Size of each octree (usually CHUNK_SIZE)
// 	int			element;
// 	int			max_depth;
// 	haven_octree_node_t	*root;  // Root for the global octree (used for non-chunked lookups)
// 	float			*node_sizes;  // Pre-calculated node sizes for each depth
    
//     // Chunking system
//     haven_chunk_t chunks[MAX_CHUNKS];
//     int chunk_count;
//     ivec3 visible_range;  // Range of chunks to keep loaded
    
//     // Memory pool
//     haven_memory_pool_t *memory_pool;
// }	haven_octree_t;

// // Raycast data structure
// typedef struct haven_raycast_result_s {
//     bool hit;
//     void *data;
//     vec3 position;
//     vec3 normal;
//     float distance;
// } haven_raycast_result_t;

// // Frustum culling data structure
// typedef struct haven_frustum_s {
//     vec4 planes[6];  // Frustum planes in world space (ax + by + cz + d = 0 format)
// } haven_frustum_t;

// // Initialize memory pool
// haven_memory_pool_t *haven_memory_pool_init() {
//     haven_memory_pool_t *pool = (haven_memory_pool_t*)malloc(sizeof(haven_memory_pool_t));
//     memset(pool->used, 0, sizeof(pool->used));
//     pool->next_free = 0;
//     pool->count = 0;
//     return pool;
// }

// // Allocate node from memory pool
// haven_octree_node_t *haven_memory_pool_alloc(haven_memory_pool_t *pool) {
//     if (pool->count >= MEMORY_POOL_SIZE) {
//         // Pool is full, expand or handle error
//         return malloc(sizeof(haven_octree_node_t));
//     }
    
//     // Find next free slot
//     while (pool->used[pool->next_free] && pool->next_free < MEMORY_POOL_SIZE) {
//         pool->next_free++;
//     }
    
//     if (pool->next_free >= MEMORY_POOL_SIZE) {
//         pool->next_free = 0;
//         // Try again from beginning
//         while (pool->used[pool->next_free] && pool->next_free < MEMORY_POOL_SIZE) {
//             pool->next_free++;
//         }
        
//         if (pool->next_free >= MEMORY_POOL_SIZE) {
//             // Really full
//             return malloc(sizeof(haven_octree_node_t));
//         }
//     }
    
//     // Mark as used and initialize
//     pool->used[pool->next_free] = true;
//     pool->count++;
//     haven_octree_node_t *node = &pool->nodes[pool->next_free];
//     node->isleaf = false;
//     node->data = NULL;
//     for (int i = 0; i < 8; i++) {
//         node->children[i] = NULL;
//     }
    
//     // Find next free for next allocation
//     pool->next_free++;
//     if (pool->next_free >= MEMORY_POOL_SIZE) {
//         pool->next_free = 0;
//     }
    
//     return node;
// }

// // Free node back to memory pool
// void haven_memory_pool_free(haven_memory_pool_t *pool, haven_octree_node_t *node) {
//     // Check if node is from pool
//     if (node >= pool->nodes && node < pool->nodes + MEMORY_POOL_SIZE) {
//         int index = (int)(node - pool->nodes);
//         pool->used[index] = false;
//         pool->count--;
        
//         // Update next_free if this is before current next_free
//         if (index < pool->next_free) {
//             pool->next_free = index;
//         }
//     } else {
//         // Node was allocated outside the pool
//         free(node);
//     }
// }

// static bool	is_point_valid(vec3 point, haven_octree_t *svo) {
// 	return (point[0] >= 0.0f && point[0] <= svo->size &&
// 		point[1] >= 0.0f && point[1] <= svo->size &&
// 		point[2] >= 0.0f && point[2] <= svo->size);
// }

// // Convert world coordinates to chunk coordinates
// void haven_world_to_chunk(vec3 world_pos, ivec3 chunk_pos) {
//     chunk_pos[0] = (int)floorf(world_pos[0] / CHUNK_SIZE);
//     chunk_pos[1] = (int)floorf(world_pos[1] / CHUNK_SIZE);
//     chunk_pos[2] = (int)floorf(world_pos[2] / CHUNK_SIZE);
// }

// // Convert world coordinates to local coordinates within a chunk
// void haven_world_to_local(vec3 world_pos, vec3 local_pos) {
//     local_pos[0] = fmodf(world_pos[0], CHUNK_SIZE);
//     if (local_pos[0] < 0) local_pos[0] += CHUNK_SIZE;
    
//     local_pos[1] = fmodf(world_pos[1], CHUNK_SIZE);
//     if (local_pos[1] < 0) local_pos[1] += CHUNK_SIZE;
    
//     local_pos[2] = fmodf(world_pos[2], CHUNK_SIZE);
//     if (local_pos[2] < 0) local_pos[2] += CHUNK_SIZE;
// }

// // Find or create a chunk at the given coordinates
// haven_chunk_t *haven_get_chunk(haven_octree_t *svo, int x, int y, int z) {
//     // First check if chunk already exists
//     for (int i = 0; i < svo->chunk_count; i++) {
//         if (svo->chunks[i].x == x && svo->chunks[i].y == y && svo->chunks[i].z == z) {
//             return &svo->chunks[i];
//         }
//     }
    
//     // If not at max capacity, create new chunk
//     if (svo->chunk_count < MAX_CHUNKS) {
//         haven_chunk_t *chunk = &svo->chunks[svo->chunk_count];
//         chunk->x = x;
//         chunk->y = y;
//         chunk->z = z;
//         chunk->active = true;
//         chunk->modified = false;
//         chunk->root = NULL;
//         svo->chunk_count++;
//         return chunk;
//     }
    
//     // At max capacity, find least recently used chunk
//     // For now, just return the first chunk (this should be improved)
//     return &svo->chunks[0];
// }

// void	haven_octree_node_delete(haven_octree_node_t *node, haven_memory_pool_t *pool) {
// 	if (!node) return;
// 	if (!node->isleaf) {
// 		for (int i = 0; i < 8; i++) {
// 			haven_octree_node_delete(node->children[i], pool);
// 		}
// 	}
// 	haven_memory_pool_free(pool, node);
// }

// void	delete_svo(haven_octree_t *svo) {
// 	if (svo) {
// 		// Free global octree
// 		haven_octree_node_delete(svo->root, svo->memory_pool);
		
// 		// Free chunk octrees
// 		for (int i = 0; i < svo->chunk_count; i++) {
// 		    if (svo->chunks[i].root) {
// 		        haven_octree_node_delete(svo->chunks[i].root, svo->memory_pool);
// 		    }
// 		}
		
// 		free(svo->node_sizes);
// 		free(svo->memory_pool);
// 		free(svo);
// 	}
// }

// haven_octree_t	*init_svo(int _size, int _depth) {
// 	haven_octree_t	*svo = (haven_octree_t *)malloc(sizeof(haven_octree_t));

// 	svo->size = _size;
// 	svo->max_depth = _depth;
// 	svo->root = NULL;
// 	svo->element = 0;
// 	svo->chunk_count = 0;
	
// 	// Set default visible range (3x3x3 chunks around player)
// 	svo->visible_range[0] = 3;
// 	svo->visible_range[1] = 3;
// 	svo->visible_range[2] = 3;
	
// 	// Initialize memory pool
// 	svo->memory_pool = haven_memory_pool_init();
	
// 	// Pre-calculate node sizes for each depth level
// 	svo->node_sizes = (float *)malloc(sizeof(float) * (_depth + 1));
// 	for (int i = 0; i <= _depth; i++) {
// 		svo->node_sizes[i] = _size / (float)(1 << i);
// 	}
	
// 	return (svo);
// }

// // Get voxel data from world coordinates
// void	*haven_octree_get_world(vec3 world_pos, haven_octree_t *svo) {
//     // Convert world position to chunk and local coordinates
//     ivec3 chunk_pos;
//     vec3 local_pos;
    
//     haven_world_to_chunk(world_pos, chunk_pos);
//     haven_world_to_local(world_pos, local_pos);
    
//     // Find the chunk
//     haven_chunk_t *chunk = haven_get_chunk(svo, chunk_pos[0], chunk_pos[1], chunk_pos[2]);
    
//     if (!chunk->active || !chunk->root) {
//         return NULL;
//     }
    
//     // Use the local position to query within the chunk
//     return haven_octree_get(local_pos, svo);
// }

// void	*haven_octree_get(vec3 point, haven_octree_t *svo) {
// 	if (!is_point_valid(point, svo)) return NULL;
	
// 	haven_octree_node_t *current = svo->root;
// 	int depth = 0;
// 	uint32_t position = 0;  // Pack x,y,z into a single 32-bit integer
	
// 	while (current && depth < svo->max_depth) {
// 		// Convert point to integer coordinates within current node
// 		float node_size = svo->node_sizes[depth];
// 		uint32_t x = (uint32_t)((point[0] - (position & 0xFF) * node_size) / node_size);
// 		uint32_t y = (uint32_t)((point[1] - ((position >> 8) & 0xFF) * node_size) / node_size);
// 		uint32_t z = (uint32_t)((point[2] - ((position >> 16) & 0xFF) * node_size) / node_size);
		
// 		// Calculate child index using bit operations
// 		uint8_t child_index = (x & 1) | ((y & 1) << 1) | ((z & 1) << 2);
		
// 		// Update position for next level
// 		position = (position << 3) | child_index;
		
// 		current = current->children[child_index];
// 		depth++;
// 	}
	
// 	return (current && depth == svo->max_depth) ? current->data : NULL;
// }

// static void	haven_octree_insert_implementation(haven_octree_t *svo, haven_octree_node_t **node, vec3 point, void *data, uint32_t position, int depth) {
// 	if (!*node) {
// 	    // Allocate from memory pool instead of malloc
// 	    *node = haven_memory_pool_alloc(svo->memory_pool);
// 	}
    
// 	if (depth == svo->max_depth) {
// 	    (*node)->isleaf = true;
// 	    (*node)->data = data;
// 	    return;
// 	}
    
// 	float node_size = svo->node_sizes[depth];
// 	uint32_t x = (uint32_t)((point[0] - (position & 0xFF) * node_size) / node_size);
// 	uint32_t y = (uint32_t)((point[1] - ((position >> 8) & 0xFF) * node_size) / node_size);
// 	uint32_t z = (uint32_t)((point[2] - ((position >> 16) & 0xFF) * node_size) / node_size);
	
// 	uint8_t child_index = (x & 1) | ((y & 1) << 1) | ((z & 1) << 2);
// 	uint32_t new_position = (position << 3) | child_index;
    
// 	haven_octree_insert_implementation(svo, &(*node)->children[child_index], point, data, new_position, depth + 1);
// }

// // Insert voxel data at world coordinates
// bool haven_octree_insert_world(vec3 world_pos, void *data, haven_octree_t *svo) {
//     // Convert world position to chunk and local coordinates
//     ivec3 chunk_pos;
//     vec3 local_pos;
    
//     haven_world_to_chunk(world_pos, chunk_pos);
//     haven_world_to_local(world_pos, local_pos);
    
//     // Get or create the chunk
//     haven_chunk_t *chunk = haven_get_chunk(svo, chunk_pos[0], chunk_pos[1], chunk_pos[2]);
//     chunk->modified = true;
    
//     // Insert into the chunk's octree using local coordinates
//     haven_octree_insert_implementation(svo, &chunk->root, local_pos, data, 0, 0);
//     return true;
// }

// bool	haven_octree_insert(haven_octree_t *svo, vec3 point, void *data) {
// 	if (!is_point_valid(point, svo)) return (false);
	
// 	if (!haven_octree_get(point, svo)) {
// 		svo->element++;
// 	}
	
// 	haven_octree_insert_implementation(svo, &svo->root, point, data, 0, 0);
// 	return (true);
// }

// // Set the visible range of chunks
// void haven_octree_set_visible_range(haven_octree_t *svo, int x_range, int y_range, int z_range) {
//     svo->visible_range[0] = x_range;
//     svo->visible_range[1] = y_range;
//     svo->visible_range[2] = z_range;
// }

// // Update visible chunks based on player position
// void haven_octree_update_visible_chunks(haven_octree_t *svo, vec3 player_pos) {
//     ivec3 center_chunk;
//     haven_world_to_chunk(player_pos, center_chunk);
    
//     // Mark all chunks as inactive initially
//     for (int i = 0; i < svo->chunk_count; i++) {
//         svo->chunks[i].active = false;
//     }
    
//     // Activate chunks within visible range
//     for (int x = center_chunk[0] - svo->visible_range[0]; x <= center_chunk[0] + svo->visible_range[0]; x++) {
//         for (int y = center_chunk[1] - svo->visible_range[1]; y <= center_chunk[1] + svo->visible_range[1]; y++) {
//             for (int z = center_chunk[2] - svo->visible_range[2]; z <= center_chunk[2] + svo->visible_range[2]; z++) {
//                 haven_chunk_t *chunk = haven_get_chunk(svo, x, y, z);
//                 chunk->active = true;
//             }
//         }
//     }
// }

// // Save chunk data to file/storage
// bool haven_octree_save_chunk(haven_octree_t *svo, int chunk_x, int chunk_y, int chunk_z, const char *filepath) {
//     // Find the chunk
//     haven_chunk_t *chunk = NULL;
//     for (int i = 0; i < svo->chunk_count; i++) {
//         if (svo->chunks[i].x == chunk_x && 
//             svo->chunks[i].y == chunk_y && 
//             svo->chunks[i].z == chunk_z) {
//             chunk = &svo->chunks[i];
//             break;
//         }
//     }
    
//     if (!chunk || !chunk->modified) {
//         return false;  // Chunk not found or not modified
//     }
    
//     // Implement serialization logic here
//     // For now, just mark as not modified
//     chunk->modified = false;
//     return true;
// }

// // Load chunk data from file/storage
// bool haven_octree_load_chunk(haven_octree_t *svo, int chunk_x, int chunk_y, int chunk_z, const char *filepath) {
//     // Get or create the chunk
//     haven_chunk_t *chunk = haven_get_chunk(svo, chunk_x, chunk_y, chunk_z);
    
//     // Implement deserialization logic here
    
//     return true;
// }

// haven_octree_node_t	*haven_octree_node_create(haven_octree_t *svo) {
//     // Use memory pool instead of malloc
//     return haven_memory_pool_alloc(svo->memory_pool);
// }

// // Raycast function for interaction with octree
// bool haven_octree_raycast(haven_octree_t *svo, vec3 origin, vec3 direction, 
//                          haven_raycast_result_t *result, float max_distance) {
//     // Initialize result
//     result->hit = false;
//     result->distance = max_distance;
    
//     if (!svo->root) return false;
    
//     // Normalize direction
//     vec3 dir;
//     glm_vec3_normalize_to(direction, dir);
    
//     // Calculate inverse direction for efficient slab testing
//     vec3 inv_dir = {
//         1.0f / (dir[0] == 0 ? 0.00001f : dir[0]),
//         1.0f / (dir[1] == 0 ? 0.00001f : dir[1]),
//         1.0f / (dir[2] == 0 ? 0.00001f : dir[2])
//     };
    
//     // Bounds of the root node
//     vec3 node_min = {0, 0, 0};
//     vec3 node_max = {svo->size, svo->size, svo->size};
    
//     // Test intersection with the root node
//     float tmin, tmax;
//     float t1, t2;
    
//     t1 = (node_min[0] - origin[0]) * inv_dir[0];
//     t2 = (node_max[0] - origin[0]) * inv_dir[0];
//     tmin = fminf(t1, t2);
//     tmax = fmaxf(t1, t2);
    
//     t1 = (node_min[1] - origin[1]) * inv_dir[1];
//     t2 = (node_max[1] - origin[1]) * inv_dir[1];
//     tmin = fmaxf(tmin, fminf(t1, t2));
//     tmax = fminf(tmax, fmaxf(t1, t2));
    
//     t1 = (node_min[2] - origin[2]) * inv_dir[2];
//     t2 = (node_max[2] - origin[2]) * inv_dir[2];
//     tmin = fmaxf(tmin, fminf(t1, t2));
//     tmax = fminf(tmax, fmaxf(t1, t2));
    
//     if (tmax < 0 || tmin > tmax) return false;
    
//     // If we intersect the root, traverse the tree
//     return haven_octree_raycast_recursive(svo, svo->root, origin, dir, inv_dir, 
//                                          node_min, node_max, 0, result);
// }

// // Recursive helper for raycast traversal
// bool haven_octree_raycast_recursive(haven_octree_t *svo, haven_octree_node_t *node, 
//                                    vec3 origin, vec3 dir, vec3 inv_dir, 
//                                    vec3 node_min, vec3 node_max, int depth, 
//                                    haven_raycast_result_t *result) {
//     if (!node) return false;
    
//     // If this is a leaf at max depth and it has data, we hit something
//     if (depth == svo->max_depth && node->isleaf && node->data) {
//         // Calculate hit position and normal
//         vec3 hit_pos;
//         float t = result->distance;
//         glm_vec3_scale(dir, t, hit_pos);
//         glm_vec3_add(origin, hit_pos, hit_pos);
        
//         // Determine normal based on which face was hit
//         vec3 normal = {0, 0, 0};
//         float epsilon = 0.001f;
        
//         if (fabsf(hit_pos[0] - node_min[0]) < epsilon) normal[0] = -1;
//         else if (fabsf(hit_pos[0] - node_max[0]) < epsilon) normal[0] = 1;
//         else if (fabsf(hit_pos[1] - node_min[1]) < epsilon) normal[1] = -1;
//         else if (fabsf(hit_pos[1] - node_max[1]) < epsilon) normal[1] = 1;
//         else if (fabsf(hit_pos[2] - node_min[2]) < epsilon) normal[2] = -1;
//         else if (fabsf(hit_pos[2] - node_max[2]) < epsilon) normal[2] = 1;
        
//         result->hit = true;
//         result->data = node->data;
//         glm_vec3_copy(hit_pos, result->position);
//         glm_vec3_copy(normal, result->normal);
        
//         return true;
//     }
    
//     // Early return if we're at max depth but no hit
//     if (depth == svo->max_depth) return false;
    
//     // Calculate center of current node
//     vec3 center;
//     glm_vec3_add(node_min, node_max, center);
//     glm_vec3_scale(center, 0.5f, center);
    
//     // Determine which child to check first (optimization to check nearest children first)
//     typedef struct child_entry {
//         int index;
//         float t_min;
//     } child_entry;
    
//     child_entry children[8];
//     int child_count = 0;
    
//     // For each potential child
//     for (int i = 0; i < 8; i++) {
//         if (!node->children[i]) continue;
        
//         // Calculate child bounds
//         vec3 child_min, child_max;
//         for (int j = 0; j < 3; j++) {
//             if (CHILD_INDEX_TABLE[i][j] == 0) {
//                 child_min[j] = node_min[j];
//                 child_max[j] = center[j];
//             } else {
//                 child_min[j] = center[j];
//                 child_max[j] = node_max[j];
//             }
//         }
        
//         // Check intersection with child bounds
//         float t1, t2, child_tmin, child_tmax;
        
//         t1 = (child_min[0] - origin[0]) * inv_dir[0];
//         t2 = (child_max[0] - origin[0]) * inv_dir[0];
//         child_tmin = fminf(t1, t2);
//         child_tmax = fmaxf(t1, t2);
        
//         t1 = (child_min[1] - origin[1]) * inv_dir[1];
//         t2 = (child_max[1] - origin[1]) * inv_dir[1];
//         child_tmin = fmaxf(child_tmin, fminf(t1, t2));
//         child_tmax = fminf(child_tmax, fmaxf(t1, t2));
        
//         t1 = (child_min[2] - origin[2]) * inv_dir[2];
//         t2 = (child_max[2] - origin[2]) * inv_dir[2];
//         child_tmin = fmaxf(child_tmin, fminf(t1, t2));
//         child_tmax = fminf(child_tmax, fmaxf(t1, t2));
        
//         // If we intersect and it's closer than current result
//         if (child_tmax >= 0 && child_tmin <= child_tmax && child_tmin < result->distance) {
//             children[child_count].index = i;
//             children[child_count].t_min = child_tmin;
//             child_count++;
//         }
//     }
    
//     // Sort children by distance (insertion sort is fine for small arrays)
//     for (int i = 1; i < child_count; i++) {
//         child_entry temp = children[i];
//         int j = i - 1;
//         while (j >= 0 && children[j].t_min > temp.t_min) {
//             children[j + 1] = children[j];
//             j--;
//         }
//         children[j + 1] = temp;
//     }
    
//     // Visit children in sorted order
//     bool hit = false;
//     for (int i = 0; i < child_count; i++) {
//         int idx = children[i].index;
        
//         // Calculate child bounds
//         vec3 child_min, child_max;
//         for (int j = 0; j < 3; j++) {
//             if (CHILD_INDEX_TABLE[idx][j] == 0) {
//                 child_min[j] = node_min[j];
//                 child_max[j] = center[j];
//             } else {
//                 child_min[j] = center[j];
//                 child_max[j] = node_max[j];
//             }
//         }
        
//         // Recursive traversal
//         if (haven_octree_raycast_recursive(svo, node->children[idx], origin, dir, inv_dir, 
//                                           child_min, child_max, depth + 1, result)) {
//             hit = true;
//             break;  // Early exit once we've found the closest hit
//         }
//     }
    
//     return hit;
// }

// // Function to find neighbors of a voxel
// bool haven_octree_get_neighbors(haven_octree_t *svo, vec3 point, void *neighbors[6]) {
//     // Initialize all neighbors to NULL
//     for (int i = 0; i < 6; i++) {
//         neighbors[i] = NULL;
//     }
    
//     if (!is_point_valid(point, svo)) return false;
    
//     // Direction vectors for 6-connectivity (+-X, +-Y, +-Z)
//     const vec3 directions[6] = {
//         {1, 0, 0}, {-1, 0, 0},
//         {0, 1, 0}, {0, -1, 0},
//         {0, 0, 1}, {0, 0, -1}
//     };
    
//     // Check each direction
//     for (int i = 0; i < 6; i++) {
//         vec3 neighbor_pos;
//         glm_vec3_add(point, directions[i], neighbor_pos);
        
//         // Get data at neighbor position
//         neighbors[i] = haven_octree_get(neighbor_pos, svo);
//     }
    
//     return true;
// }

// // Function to find world neighbors (handles chunk boundaries)
// bool haven_octree_get_world_neighbors(haven_octree_t *svo, vec3 world_pos, void *neighbors[6]) {
//     // Initialize all neighbors to NULL
//     for (int i = 0; i < 6; i++) {
//         neighbors[i] = NULL;
//     }
    
//     // Direction vectors for 6-connectivity (+-X, +-Y, +-Z)
//     const vec3 directions[6] = {
//         {1, 0, 0}, {-1, 0, 0},
//         {0, 1, 0}, {0, -1, 0},
//         {0, 0, 1}, {0, 0, -1}
//     };
    
//     // Check each direction
//     for (int i = 0; i < 6; i++) {
//         vec3 neighbor_pos;
//         glm_vec3_add(world_pos, directions[i], neighbor_pos);
        
//         // Get data at neighbor position (handles chunk boundaries automatically)
//         neighbors[i] = haven_octree_get_world(neighbor_pos, svo);
//     }
    
//     return true;
// }

// // Apply a function to each voxel in a region
// void haven_octree_region_foreach(haven_octree_t *svo, vec3 min_point, vec3 max_point, 
//                                 void (*callback)(vec3 pos, void *data, void *user_data), 
//                                 void *user_data) {
//     // Convert to chunk coordinates
//     ivec3 min_chunk, max_chunk;
//     haven_world_to_chunk(min_point, min_chunk);
//     haven_world_to_chunk(max_point, max_chunk);
    
//     // Iterate over all chunks in range
//     for (int cx = min_chunk[0]; cx <= max_chunk[0]; cx++) {
//         for (int cy = min_chunk[1]; cy <= max_chunk[1]; cy++) {
//             for (int cz = min_chunk[2]; cz <= max_chunk[2]; cz++) {
//                 // Find this chunk
//                 haven_chunk_t *chunk = NULL;
//                 for (int i = 0; i < svo->chunk_count; i++) {
//                     if (svo->chunks[i].x == cx && 
//                         svo->chunks[i].y == cy && 
//                         svo->chunks[i].z == cz) {
//                         chunk = &svo->chunks[i];
//                         break;
//                     }
//                 }
                
//                 if (!chunk || !chunk->active || !chunk->root) {
//                     continue;  // Skip inactive or empty chunks
//                 }
                
//                 // Calculate chunk boundaries in world coordinates
//                 vec3 chunk_min = {cx * CHUNK_SIZE, cy * CHUNK_SIZE, cz * CHUNK_SIZE};
//                 vec3 chunk_max = {(cx + 1) * CHUNK_SIZE, (cy + 1) * CHUNK_SIZE, (cz + 1) * CHUNK_SIZE};
                
//                 // Clamp to region boundaries
//                 vec3 region_min, region_max;
//                 for (int i = 0; i < 3; i++) {
//                     region_min[i] = fmaxf(min_point[i], chunk_min[i]);
//                     region_max[i] = fminf(max_point[i], chunk_max[i]);
//                 }
                
//                 // Iterate through voxels in this chunk's region
//                 for (float x = region_min[0]; x <= region_max[0]; x += 1.0f) {
//                     for (float y = region_min[1]; y <= region_max[1]; y += 1.0f) {
//                         for (float z = region_min[2]; z <= region_max[2]; z += 1.0f) {
//                             vec3 pos = {x, y, z};
//                             void *data = haven_octree_get_world(pos, svo);
//                             if (data) {
//                                 callback(pos, data, user_data);
//                             }
//                         }
//                     }
//                 }
//             }
//         }
//     }
// }

// // Level of Detail (LOD) generation for distant rendering
// typedef struct haven_octree_lod_s {
//     int level;  // LOD level (0 = highest detail)
//     haven_octree_node_t *root;  // Root of LOD octree
// } haven_octree_lod_t;

// // Generate LOD representation for a chunk
// void haven_octree_generate_lod(haven_octree_t *svo, haven_chunk_t *chunk) {
//     // For each non-leaf node, we can calculate aggregate data from children
//     // This is a simple implementation - a real voxel engine would use more sophisticated LOD generation
    
//     // To be implemented: recursively combine child voxels to create parent voxels
//     // You would typically do this by merging attributes like color/material/etc.
// }

// // Get voxel data at a specific LOD level
// void *haven_octree_get_lod(haven_octree_t *svo, vec3 point, int lod_level) {
//     // Start from the global octree
//     haven_octree_node_t *current = svo->root;
//     if (!current) return NULL;
    
//     // Calculate depth to stop at based on LOD level
//     int target_depth = svo->max_depth - lod_level;
//     if (target_depth < 0) target_depth = 0;
    
//     int depth = 0;
//     uint32_t position = 0;
    
//     // Traverse until we reach the target depth or a leaf node
//     while (current && !current->isleaf && depth < target_depth) {
//         float node_size = svo->node_sizes[depth];
//         uint32_t x = (uint32_t)((point[0] - (position & 0xFF) * node_size) / node_size);
//         uint32_t y = (uint32_t)((point[1] - ((position >> 8) & 0xFF) * node_size) / node_size);
//         uint32_t z = (uint32_t)((point[2] - ((position >> 16) & 0xFF) * node_size) / node_size);
        
//         uint8_t child_index = (x & 1) | ((y & 1) << 1) | ((z & 1) << 2);
//         position = (position << 3) | child_index;
        
//         current = current->children[child_index];
//         depth++;
//     }
    
//     // Return data at this node if it exists
//     return current ? current->data : NULL;
// }

// // Initialize frustum from projection and view matrices
// void haven_frustum_from_matrix(mat4 proj_view, haven_frustum_t *frustum) {
//     // Extract the 6 planes from the combined projection-view matrix
//     // Left plane
//     frustum->planes[0][0] = proj_view[0][3] + proj_view[0][0];
//     frustum->planes[0][1] = proj_view[1][3] + proj_view[1][0];
//     frustum->planes[0][2] = proj_view[2][3] + proj_view[2][0];
//     frustum->planes[0][3] = proj_view[3][3] + proj_view[3][0];
    
//     // Right plane
//     frustum->planes[1][0] = proj_view[0][3] - proj_view[0][0];
//     frustum->planes[1][1] = proj_view[1][3] - proj_view[1][0];
//     frustum->planes[1][2] = proj_view[2][3] - proj_view[2][0];
//     frustum->planes[1][3] = proj_view[3][3] - proj_view[3][0];
    
//     // Bottom plane
//     frustum->planes[2][0] = proj_view[0][3] + proj_view[0][1];
//     frustum->planes[2][1] = proj_view[1][3] + proj_view[1][1];
//     frustum->planes[2][2] = proj_view[2][3] + proj_view[2][1];
//     frustum->planes[2][3] = proj_view[3][3] + proj_view[3][1];
    
//     // Top plane
//     frustum->planes[3][0] = proj_view[0][3] - proj_view[0][1];
//     frustum->planes[3][1] = proj_view[1][3] - proj_view[1][1];
//     frustum->planes[3][2] = proj_view[2][3] - proj_view[2][1];
//     frustum->planes[3][3] = proj_view[3][3] - proj_view[3][1];
    
//     // Near plane
//     frustum->planes[4][0] = proj_view[0][3] + proj_view[0][2];
//     frustum->planes[4][1] = proj_view[1][3] + proj_view[1][2];
//     frustum->planes[4][2] = proj_view[2][3] + proj_view[2][2];
//     frustum->planes[4][3] = proj_view[3][3] + proj_view[3][2];
    
//     // Far plane
//     frustum->planes[5][0] = proj_view[0][3] - proj_view[0][2];
//     frustum->planes[5][1] = proj_view[1][3] - proj_view[1][2];
//     frustum->planes[5][2] = proj_view[2][3] - proj_view[2][2];
//     frustum->planes[5][3] = proj_view[3][3] - proj_view[3][2];
    
//     // Normalize all planes
//     for (int i = 0; i < 6; i++) {
//         float length = sqrtf(frustum->planes[i][0] * frustum->planes[i][0] +
//                             frustum->planes[i][1] * frustum->planes[i][1] +
//                             frustum->planes[i][2] * frustum->planes[i][2]);
        
//         frustum->planes[i][0] /= length;
//         frustum->planes[i][1] /= length;
//         frustum->planes[i][2] /= length;
//         frustum->planes[i][3] /= length;
//     }
// }

// // Test if a point is inside the frustum
// bool haven_frustum_test_point(haven_frustum_t *frustum, vec3 point) {
//     for (int i = 0; i < 6; i++) {
//         if (frustum->planes[i][0] * point[0] +
//             frustum->planes[i][1] * point[1] +
//             frustum->planes[i][2] * point[2] +
//             frustum->planes[i][3] <= 0) {
//             return false;  // Outside this plane
//         }
//     }
//     return true;  // Inside all planes
// }

// // Test if a sphere is inside the frustum
// bool haven_frustum_test_sphere(haven_frustum_t *frustum, vec3 center, float radius) {
//     for (int i = 0; i < 6; i++) {
//         float distance = frustum->planes[i][0] * center[0] +
//                         frustum->planes[i][1] * center[1] +
//                         frustum->planes[i][2] * center[2] +
//                         frustum->planes[i][3];
        
//         if (distance <= -radius) {
//             return false;  // Outside this plane (behind by more than radius)
//         }
//     }
//     return true;  // At least intersecting all planes
// }

// // Test if an axis-aligned box is inside the frustum
// bool haven_frustum_test_aabb(haven_frustum_t *frustum, vec3 min_point, vec3 max_point) {
//     // Check each plane
//     for (int i = 0; i < 6; i++) {
//         // Check if box is completely outside this plane
//         vec3 p;
//         vec3 n;
        
//         // Choose the corner that's furthest in the direction of the plane normal
//         n[0] = frustum->planes[i][0];
//         n[1] = frustum->planes[i][1];
//         n[2] = frustum->planes[i][2];
        
//         p[0] = (n[0] > 0) ? max_point[0] : min_point[0];
//         p[1] = (n[1] > 0) ? max_point[1] : min_point[1];
//         p[2] = (n[2] > 0) ? max_point[2] : min_point[2];
        
//         // If this most positive point is outside, the whole box is outside
//         if (n[0] * p[0] + n[1] * p[1] + n[2] * p[2] + frustum->planes[i][3] < 0) {
//             return false;
//         }
//     }
    
//     return true;  // At least intersecting all planes
// }

// // Test if a chunk is visible in the frustum
// bool haven_frustum_test_chunk(haven_frustum_t *frustum, haven_chunk_t *chunk) {
//     vec3 min_point = {
//         chunk->x * CHUNK_SIZE,
//         chunk->y * CHUNK_SIZE,
//         chunk->z * CHUNK_SIZE
//     };
    
//     vec3 max_point = {
//         (chunk->x + 1) * CHUNK_SIZE,
//         (chunk->y + 1) * CHUNK_SIZE,
//         (chunk->z + 1) * CHUNK_SIZE
//     };
    
//     return haven_frustum_test_aabb(frustum, min_point, max_point);
// }

// // Get all visible chunks based on camera frustum
// int haven_octree_get_visible_chunks(haven_octree_t *svo, haven_frustum_t *frustum, 
//                                   haven_chunk_t **visible_chunks, int max_visible) {
//     int count = 0;
    
//     for (int i = 0; i < svo->chunk_count && count < max_visible; i++) {
//         if (svo->chunks[i].active && svo->chunks[i].root) {
//             if (haven_frustum_test_chunk(frustum, &svo->chunks[i])) {
//                 visible_chunks[count++] = &svo->chunks[i];
//             }
//         }
//     }
    
//     return count;
// }

// // Octree traversal with frustum culling for efficient rendering
// void haven_octree_render_recursive(haven_octree_t *svo, haven_octree_node_t *node, 
//                                  int depth, vec3 min_point, vec3 max_point, 
//                                  haven_frustum_t *frustum,
//                                  void (*render_callback)(vec3 min, vec3 max, void *data, void *user_data),
//                                  void *user_data) {
//     // Early exit if node is null
//     if (!node) return;
    
//     // Test if this node's bounding box is visible
//     if (!haven_frustum_test_aabb(frustum, min_point, max_point)) {
//         return;  // Skip this node and all its children
//     }
    
//     // If this is a leaf node with data, render it
//     if (depth == svo->max_depth && node->isleaf && node->data) {
//         render_callback(min_point, max_point, node->data, user_data);
//         return;
//     }
    
//     // If we're at max depth but no data, or if it's a leaf node, stop
//     if (depth == svo->max_depth || node->isleaf) {
//         return;
//     }
    
//     // Calculate center of current node
//     vec3 center;
//     glm_vec3_add(min_point, max_point, center);
//     glm_vec3_scale(center, 0.5f, center);
    
//     // Recursive traversal of children
//     for (int i = 0; i < 8; i++) {
//         if (!node->children[i]) continue;
        
//         // Calculate child bounds
//         vec3 child_min, child_max;
//         for (int j = 0; j < 3; j++) {
//             if (CHILD_INDEX_TABLE[i][j] == 0) {
//                 child_min[j] = min_point[j];
//                 child_max[j] = center[j];
//             } else {
//                 child_min[j] = center[j];
//                 child_max[j] = max_point[j];
//             }
//         }
        
//         // Recursive traversal with frustum culling
//         haven_octree_render_recursive(svo, node->children[i], depth + 1, 
//                                     child_min, child_max, frustum,
//                                     render_callback, user_data);
//     }
// }

// // High-level function to render a chunk with frustum culling
// void haven_octree_render_chunk(haven_octree_t *svo, haven_chunk_t *chunk,
//                              haven_frustum_t *frustum,
//                              void (*render_callback)(vec3 min, vec3 max, void *data, void *user_data),
//                              void *user_data) {
//     if (!chunk->active || !chunk->root) return;
    
//     // Calculate chunk boundaries
//     vec3 min_point = {
//         chunk->x * CHUNK_SIZE,
//         chunk->y * CHUNK_SIZE,
//         chunk->z * CHUNK_SIZE
//     };
    
//     vec3 max_point = {
//         (chunk->x + 1) * CHUNK_SIZE,
//         (chunk->y + 1) * CHUNK_SIZE,
//         (chunk->z + 1) * CHUNK_SIZE
//     };
    
//     // First check if entire chunk is visible
//     if (!haven_frustum_test_aabb(frustum, min_point, max_point)) {
//         return;  // Skip this chunk entirely
//     }
    
//     // Traverse and render the chunk
//     haven_octree_render_recursive(svo, chunk->root, 0, min_point, max_point, 
//                                 frustum, render_callback, user_data);
// }

// // Render all visible chunks with frustum culling
// void haven_octree_render(haven_octree_t *svo, vec3 camera_pos, mat4 view_proj,
//                        void (*render_callback)(vec3 min, vec3 max, void *data, void *user_data),
//                        void *user_data) {
//     // Create and initialize frustum from view-projection matrix
//     haven_frustum_t frustum;
//     haven_frustum_from_matrix(view_proj, &frustum);
    
//     // Get visible range of chunks based on player position
//     ivec3 center_chunk;
//     haven_world_to_chunk(camera_pos, center_chunk);
    
//     // Render chunks in visible range with frustum culling
//     for (int x = center_chunk[0] - svo->visible_range[0]; 
//          x <= center_chunk[0] + svo->visible_range[0]; x++) {
//         for (int y = center_chunk[1] - svo->visible_range[1]; 
//              y <= center_chunk[1] + svo->visible_range[1]; y++) {
//             for (int z = center_chunk[2] - svo->visible_range[2]; 
//                  z <= center_chunk[2] + svo->visible_range[2]; z++) {
                
//                 // Find this chunk
//                 haven_chunk_t *chunk = NULL;
//                 for (int i = 0; i < svo->chunk_count; i++) {
//                     if (svo->chunks[i].x == x && 
//                         svo->chunks[i].y == y && 
//                         svo->chunks[i].z == z && 
//                         svo->chunks[i].active) {
//                         chunk = &svo->chunks[i];
//                         break;
//                     }
//                 }
                
//                 if (chunk && chunk->root) {
//                     haven_octree_render_chunk(svo, chunk, &frustum, render_callback, user_data);
//                 }
//             }
//         }
//     }
// }

// // Sample distance-based LOD selection function
// int haven_octree_select_lod(float distance) {
//     // Simple distance-based LOD selection
//     if (distance < 32.0f) return 0;       // Full detail
//     else if (distance < 64.0f) return 1;  // Half detail
//     else if (distance < 128.0f) return 2; // Quarter detail
//     else if (distance < 256.0f) return 3; // Eighth detail
//     else return 4;                       // Minimum detail
// }

// // Render with automatic LOD selection based on distance
// void haven_octree_render_with_lod(haven_octree_t *svo, vec3 camera_pos, mat4 view_proj,
//                                 void (*render_callback)(vec3 min, vec3 max, void *data, int lod, void *user_data),
//                                 void *user_data) {
//     // Create and initialize frustum from view-projection matrix
//     haven_frustum_t frustum;
//     haven_frustum_from_matrix(view_proj, &frustum);
    
//     // Get visible range of chunks based on player position
//     ivec3 center_chunk;
//     haven_world_to_chunk(camera_pos, center_chunk);
    
//     // Render chunks with LOD selection
//     for (int x = center_chunk[0] - svo->visible_range[0]; 
//          x <= center_chunk[0] + svo->visible_range[0]; x++) {
//         for (int y = center_chunk[1] - svo->visible_range[1]; 
//              y <= center_chunk[1] + svo->visible_range[1]; y++) {
//             for (int z = center_chunk[2] - svo->visible_range[2]; 
//                  z <= center_chunk[2] + svo->visible_range[2]; z++) {
                
//                 // Find this chunk
//                 haven_chunk_t *chunk = NULL;
//                 for (int i = 0; i < svo->chunk_count; i++) {
//                     if (svo->chunks[i].x == x && 
//                         svo->chunks[i].y == y && 
//                         svo->chunks[i].z == z && 
//                         svo->chunks[i].active) {
//                         chunk = &svo->chunks[i];
//                         break;
//                     }
//                 }
                
//                 if (!chunk || !chunk->root) continue;
                
//                 // Calculate chunk center
//                 vec3 chunk_center = {
//                     (x + 0.5f) * CHUNK_SIZE,
//                     (y + 0.5f) * CHUNK_SIZE,
//                     (z + 0.5f) * CHUNK_SIZE
//                 };
                
//                 // Calculate distance to camera
//                 vec3 diff;
//                 glm_vec3_sub(chunk_center, camera_pos, diff);
//                 float distance = glm_vec3_norm(diff);
                
//                 // Select appropriate LOD level based on distance
//                 int lod = haven_octree_select_lod(distance);
                
//                 // Render with the selected LOD level
//                 // This would be implemented by rendering fewer octree nodes, 
//                 // or using pre-generated lower-resolution versions
//             }
//         }
//     }
// }

