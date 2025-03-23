#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <cglm/cglm.h>
#include <math.h>

// Pre-calculated lookup table for child indices based on point position
static const uint8_t CHILD_INDEX_TABLE[8][3] = {
    {0, 0, 0}, {1, 0, 0}, {0, 1, 0}, {1, 1, 0},
    {0, 0, 1}, {1, 0, 1}, {0, 1, 1}, {1, 1, 1}
};

typedef struct	haven_octree_node_s {
	bool				isleaf;
	void				*data;
	struct haven_octree_node_s	*children[8];
}	haven_octree_node_t;

typedef	struct	haven_octree_s {
	int			size;
	int			element;
	int			max_depth;
	haven_octree_node_t	*root;
	float			*node_sizes;  // Pre-calculated node sizes for each depth
}	haven_octree_t;

static bool	is_point_valid(vec3 point, haven_octree_t *svo) {
	return (point[0] >= 0.0f && point[0] <= svo->size &&
		point[1] >= 0.0f && point[1] <= svo->size &&
		point[2] >= 0.0f && point[2] <= svo->size);
}

void	haven_octree_node_delete(haven_octree_node_t *node) {
	if (!node) return;
	if (!node->isleaf) {
		for (int i = 0; i < 8; i++) {
			haven_octree_node_delete(node->children[i]);
		}
	}
	free(node);
}

void	delete_svo(haven_octree_t *svo) {
	if (svo) {
		haven_octree_node_delete(svo->root);
		free(svo->node_sizes);
		free(svo);
	}
}

haven_octree_t	*init_svo(int _size, int _depth) {
	haven_octree_t	*svo = (haven_octree_t *)malloc(sizeof(haven_octree_t));

	svo->size = _size;
	svo->max_depth = _depth;
	svo->root = NULL;
	svo->element = 0;
	
	// Pre-calculate node sizes for each depth level
	svo->node_sizes = (float *)malloc(sizeof(float) * (_depth + 1));
	for (int i = 0; i <= _depth; i++) {
		svo->node_sizes[i] = _size / (float)(1 << i);
	}
	
	return (svo);
}

void	*haven_octree_get(vec3 point, haven_octree_t *svo) {
	if (!is_point_valid(point, svo)) return NULL;
	
	haven_octree_node_t *current = svo->root;
	int depth = 0;
	uint32_t position = 0;  // Pack x,y,z into a single 32-bit integer
	
	while (current && depth < svo->max_depth) {
		// Convert point to integer coordinates within current node
		float node_size = svo->node_sizes[depth];
		uint32_t x = (uint32_t)((point[0] - (position & 0xFF) * node_size) / node_size);
		uint32_t y = (uint32_t)((point[1] - ((position >> 8) & 0xFF) * node_size) / node_size);
		uint32_t z = (uint32_t)((point[2] - ((position >> 16) & 0xFF) * node_size) / node_size);
		
		// Calculate child index using bit operations
		uint8_t child_index = (x & 1) | ((y & 1) << 1) | ((z & 1) << 2);
		
		// Update position for next level
		position = (position << 3) | child_index;
		
		current = current->children[child_index];
		depth++;
	}
	
	return (current && depth == svo->max_depth) ? current->data : NULL;
}

static void	haven_octree_insert_implementation(haven_octree_t *svo, haven_octree_node_t **node, vec3 point, void *data, uint32_t position, int depth) {
	if (!*node) {
	    *node = (haven_octree_node_t *)malloc(sizeof(haven_octree_node_t));
	    (*node)->isleaf = false;
	    (*node)->data = NULL;
	    for (int i = 0; i < 8; i++) {
		(*node)->children[i] = NULL;
	    }
	}
    
	if (depth == svo->max_depth) {
	    (*node)->isleaf = true;
	    (*node)->data = data;
	    return;
	}
    
	float node_size = svo->node_sizes[depth];
	uint32_t x = (uint32_t)((point[0] - (position & 0xFF) * node_size) / node_size);
	uint32_t y = (uint32_t)((point[1] - ((position >> 8) & 0xFF) * node_size) / node_size);
	uint32_t z = (uint32_t)((point[2] - ((position >> 16) & 0xFF) * node_size) / node_size);
	
	uint8_t child_index = (x & 1) | ((y & 1) << 1) | ((z & 1) << 2);
	uint32_t new_position = (position << 3) | child_index;
    
	haven_octree_insert_implementation(svo, &(*node)->children[child_index], point, data, new_position, depth + 1);
}

bool	haven_octree_insert(haven_octree_t *svo, vec3 point, void *data) {
	if (!is_point_valid(point, svo)) return (false);
	
	if (!haven_octree_get(point, svo)) {
		svo->element++;
	}
	
	haven_octree_insert_implementation(svo, &svo->root, point, data, 0, 0);
	return (true);
}

haven_octree_node_t	*haven_octree_node_create(void) {
	haven_octree_node_t *node = (haven_octree_node_t *)malloc(sizeof(haven_octree_node_t));
	if (!node) return NULL;
	node->isleaf = false;
	node->data = NULL;

	for (int i = 0; i < 8; i++) {
		node->children[i] = NULL;
	}

	return (node);
}

