#ifndef SVO_H
# define SVO_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>

typedef struct	s_svo_node {
	bool		isleaf;
	void		*data;
	svo_node_t	*children[8];
}	svo_node_t;

typedef	struct s_sparse_voxel_octree {
	int		size;
	int		element;
	int		max_depth;
	svo_node_t	*root;
}	svo_t;

svo_t	*init_svo(int size_, int max_depth_) {
	svo_t	*svo = (svo_t *)malloc(sizeof(svo_t));

	svo->size = size_;
	svo->max_depth = max_depth_;

	return (svo);
}

void	delete_svo() {

}

void	svo_insert_impl(svo_t *svo, svo_node_t **node, float point[3], void *data, int position[3], int depth) {
	if (!*node) {
		*node = (svo_node_t*)malloc(sizeof(svo_node_t));
	}

	(*node)->data = data;
	if (depth  == svo->max_depth) {
		(*node)->isleaf = true;
		return;
	}

	float	size = svo->size / pow(2, depth);
	
	int	child_position[3] = {
		point[0] >= (size * position[0] + (size/2.0f)),
		point[1] >= (size * position[1] + (size/2.0f)),
		point[2] >= (size * position[2] + (size/2.0f))
	};

	int	child_index = (child_position[0] << 0) | (child_position[1] << 1) | (child_position[2] << 2);

	position = (int [3]){
		(position[0] << 1) | child_position[0],
		(position[1] << 1) | child_position[1],
		(position[2] << 1) | child_position[2]
	};

	svo_insert_impl(svo, &(*node)->children[child_index], point, data, position, ++depth);
}

void	svo_insert(float point[3], void *data, svo_t *svo) {
	svo_insert_impl(svo, &svo->root, point, data, (float [3]){0, 0, 0}, 0);
}

#endif
