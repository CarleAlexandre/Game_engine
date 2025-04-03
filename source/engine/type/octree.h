#ifndef OCTREE_H
# define OCTREE_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct octree_node {
	uint8_t		child_mask;
	float		center[3];
	float		extent;
	void*		data;
	octree_node*	children[8];
}	octree_node;

typedef struct octree {
	uint8_t		depth;
	uint32_t	size;
	octree_node	root;
}	octree;

/***/
octree_node*	octree_node_create(float center[3], float extent) {
	octree_node* node = (octree_node*)malloc(sizeof(octree_node));
	assert(node);
	memcpy(node->center, center, sizeof(float) * 3);
	node->extent = extent;
	node->child_mask = 0;
	node->data = 0x00;
	memset(node->children, 0, sizeof(octree_node *) * 8);
	return (node);
}

/***/
int	octree_node_child_index(octree_node* node, float point[3]) {
	assert(node);
	int index = 0;
	if (point[0] >= node->center[0]) {
		index |= 1;
	}
	if (point[1] >= node->center[1]) {
		index |= 2;
	}
	if (point[2] >= node->center[2]) {
		index |= 4;
	}
	return (index);
}

/***/
bool	octree_node_isleaf(octree_node* node, int index) {
	assert(node);
	return ((node->child_mask & (1 << index)) != 0);
}

/***/
octree_node*	octree_node_get_or_create(octree_node* parent, int index) {
	assert(parent);
	if (parent->children[index]) {
		return (parent->children[index]);
	}

	float half = parent->extent * 0.5f;
	float child_center[3] = {
		parent->center[0] + ((index & 1) ? half : -half),
		parent->center[1] + ((index & 2) ? half : -half),
		parent->center[2] + ((index & 4) ? half : -half),
	};
	octree_node* child = octree_node_create(child_center, half);
	assert(child);

	parent->children[index] = child;
	parent->child_mask |= (1 << index);
	return (child);
}

octree_node* octree_node_lookup(octree_node* root, float point[3]) {
	assert(root);
	octree_node* current = root;

	while(current) {
		int idx = octree_node_child_index(current, point);
		if (!octree_node_isleaf(current, idx)) {
			break;
		}
		current = current->children[idx];
	}
	return (current);
}

void*	octree_node_insert(octree_node* root, float point[3], void* data, float min_extent) {
	assert(root);
	octree_node* current = root;
	while(current->extent > min_extent) {
		int idx = octree_node_child_index(current, point);
		if (!octree_node_isleaf(current, idx) && !octree_node_get_or_create(current, idx)) {
				return (NULL);
		}
		current = current->children[idx];
	}
	void* old_data = current->data;
	current->data = data;
	return (old_data);
}

void	octree_node_free(octree_node *node) {
	if (!node) {
		return;
	}
	for (int i = 0; i < 8; i++) {
		if (node->children[i]) {
			node_free(node->children[i]);
		}
	}
	free(node);
}

/***/
octree_node*	octree_traversal() {
	octree_node* node = 0x00;

	return (node);
}

/***/
bool	octree_insert() {

	return (false);
}

/***/
void*	octree_extract() {

	return (0x00);
}

octree* octree_init(const uint8_t max_depth) {
	octree* tree = 0x00;


	return (tree);
}

#endif