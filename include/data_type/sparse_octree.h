#ifndef SVO_H
# define SVO_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <raymath.h>

typedef struct	s_svo_node {
	bool			isleaf;
	void			*data;
	struct s_svo_node	*children[8];
}	svo_node_t;

typedef	struct	s_sparse_voxel_octree {
	int		size;
	int		element;
	int		max_depth;
	svo_node_t	*root;
}	svo_t;

static void	delete_node(svo_node_t *node) {
	if (!node) return;
	if (!node->isleaf) {
	    for (int i = 0; i < 8; i++) {
		delete_node(node->children[i]);
	    }
	}
	free(node);
}

static void	svo_insert_impl(svo_t *svo, svo_node_t **node, Vector3 point, void *data, int position[3], int depth) {
	if (!*node) {
	    *node = (svo_node_t *)malloc(sizeof(svo_node_t));
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
	    (point.x >= mid_x),
	    (point.y >= mid_y),
	    (point.z >= mid_z)
	};
    
	int child_index = (child_position[0] << 0) | (child_position[1] << 1) | (child_position[2] << 2);
    
	int new_position[3] = {
	    (position[0] << 1) | child_position[0],
	    (position[1] << 1) | child_position[1],
	    (position[2] << 1) | child_position[2]
	};
    
	svo_insert_impl(svo, &(*node)->children[child_index], point, data, new_position, depth + 1);
}

static bool	is_point_valid(Vector3 point, svo_t *svo) {
	return (point.x >= 0.0f && point.x <= svo->size &&
		point.y >= 0.0f && point.y <= svo->size &&
		point.z >= 0.0f && point.z <= svo->size);
}

static svo_node_t	*svo_get_node_impl(Vector3 point, svo_t *svo) {
	svo_node_t *current = svo->root;
	int depth = 0;
	int position[3] = {0, 0, 0};
	float node_size = svo->size;
    
	while (current && depth < svo->max_depth) {
	    float mid_x = position[0] * node_size + node_size / 2.0f;
	    float mid_y = position[1] * node_size + node_size / 2.0f;
	    float mid_z = position[2] * node_size + node_size / 2.0f;
    
	    int child_x = (point.x >= mid_x);
	    int child_y = (point.y >= mid_y);
	    int child_z = (point.z >= mid_z);
    
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

static svo_t	*init_svo(int size_, int max_depth_) {
	svo_t	*svo = (svo_t *)malloc(sizeof(svo_t));

	svo->size = size_;
	svo->max_depth = max_depth_;
	svo->root = NULL;
	svo->element = 0;

	return (svo);
}

static void	delete_svo(svo_t *svo) {
	if (svo) {
	    delete_node(svo->root);
	    free(svo);
	}
}

static svo_node_t	*svo_get_node(Vector3 point, svo_t *svo) {
	if (!is_point_valid(point, svo)) return (NULL);
	return (svo_get_node_impl(point, svo));
}

static bool	svo_insert(Vector3 point, void *data, svo_t *svo) {
	if (!is_point_valid(point, svo)) return (false);
	if (!svo_get_node(point, svo)) svo->element++;
	svo_insert_impl(svo, &svo->root, point, data, (int [3]){0, 0, 0}, 0);
	return (true);
}

static void	*svo_get_data(Vector3 point, svo_t *svo) {
	svo_node_t *node = svo_get_node(point, svo);
	if (node) return (node->data);
	return (0x00);
}

static bool	is_node_valid(svo_node_t *node) {
	return node && node->data;
}

#endif
