
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <cglm/cglm.h>
#include <math.h>

typedef struct	haven_octree_node_s {
	bool			isleaf;
	void			*data;
	haven_octree_node_t	*children[8];
}	haven_octree_node_t;

typedef	struct	haven_octree_s {
	int			size;
	int			element;
	int			max_depth;
	haven_octree_node_t	*root;
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
		free(svo);
	}
}

haven_octree_t	*init_svo(int _size, int _depth) {
	haven_octree_t	*svo = (haven_octree_t *)malloc(sizeof(haven_octree_t));

	svo->size = _size;
	svo->max_depth = _depth;
	svo->root = NULL;
	svo->element = 0;
	return (svo);
}

static void	haven_octree_insert_implementation(haven_octree_t *svo, haven_octree_node_t **node, vec3 point, void *data, int position[3], int depth) {
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
    
	float node_size = svo->size / (float)(1 << depth);
	float mid_x = position[0] * node_size + node_size / 2.0f;
	float mid_y = position[1] * node_size + node_size / 2.0f;
	float mid_z = position[2] * node_size + node_size / 2.0f;
    
	int child_position[3] = {
	    (point[0] >= mid_x),
	    (point[1] >= mid_y),
	    (point[2] >= mid_z)
	};
    
	int child_index = (child_position[0] << 0) | (child_position[1] << 1) | (child_position[2] << 2);
    
	int new_position[3] = {
	    (position[0] << 1) | child_position[0],
	    (position[1] << 1) | child_position[1],
	    (position[2] << 1) | child_position[2]
	};
    
	haven_octree_insert_implementation(svo, &(*node)->children[child_index], point, data, new_position, depth + 1);
}

bool	haven_octree_insert(haven_octree_t *svo, vec3 point, void *data) {
	if (!is_point_valid(point, svo)) return (false);
	// if (!svo_get_node(point, svo)) svo->element++;
		//need to change
	haven_octree_insert_implementation(svo, &svo->root, point, data, (int [3]){0, 0, 0}, 0);
	return (true);
}


void	*haven_octree_get(vec3 point, haven_octree_t *svo) {
	haven_octree_node_t *current = svo->root;
	int depth = 0;
	int position[3] = {0, 0, 0};
	float node_size = svo->size;
    
	while (current && depth < svo->max_depth) {
	    float mid_x = position[0] * node_size + node_size / 2.0f;
	    float mid_y = position[1] * node_size + node_size / 2.0f;
	    float mid_z = position[2] * node_size + node_size / 2.0f;
    
	    int child_x = (point[0] >= mid_x);
	    int child_y = (point[1] >= mid_y);
	    int child_z = (point[2] >= mid_z);
    
	    int child_index = (child_x << 0) | (child_y << 1) | (child_z << 2);
    
	    position[0] = (position[0] << 1) | child_x;
	    position[1] = (position[1] << 1) | child_y;
	    position[2] = (position[2] << 1) | child_z;
    
	    current = current->children[child_index];
	    depth++;
	    node_size /= 2.0f;
	}
    
	return (current && depth == svo->max_depth) ? current : NULL;
}
