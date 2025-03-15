#ifndef HAVEN_TYPE_H
# define HAVEN_TYPE_H

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

/**
 * @brief 
 * 
 * @param point 
 * @param svo 
 * @return true 
 * @return false 
 */
static bool	is_point_valid(vec3 point, haven_octree_t *svo);

/**
 * @brief 
 * 
 * @param node 
 */
void	haven_octree_node_delete(haven_octree_node_t *node);
/**
 * @brief 
 * 
 * @param svo 
 */
void	delete_svo(haven_octree_t *svo);

/**
 * @brief 
 * 
 * @param _size 
 * @param _depth 
 * @return haven_octree_t* 
 */
haven_octree_t	*init_svo(int _size, int _depth);

/**
 * @brief 
 * 
 * @param svo 
 * @param point 
 * @param data 
 * @return true 
 * @return false 
 */
bool	haven_octree_insert(haven_octree_t *svo, vec3 point, void *data);

/**
 * @brief 
 * 
 * @param point 
 * @param svo 
 * @return void* 
 */
void	*haven_octree_get(vec3 point, haven_octree_t *svo);



/**
 * @brief static queue data structure
 * 
 */
typedef struct	haven_squeue_s {
	void		*data; // Dynamic byte buffer
	unsigned int	front;
	unsigned int	rear;
	unsigned int	size;
	unsigned int	data_size; // Use larger type (not limited to 255 bytes)
	unsigned int	capacity;
}	haven_squeue_t;

/**
 * @brief initialize a static queue (max 100 element of data_size per queue)
 * 
 * @param q 
 * @param data_size 
 */
void	haven_squeue_init(haven_squeue_t *q, unsigned int data_size);

/**
 * @brief 
 * 
 * @param q 
 * @param value 
 * @return int 
 */
int	haven_squeue_push(haven_squeue_t *q, const void *value);

/**
 * @brief 
 * 
 * @param q 
 * @param value 
 * @return int 
 */
int	haven_squeue_pop(haven_squeue_t *q, void *value);

/**
 * @brief 
 * 
 * @param q 
 */
void	haven_squeue_destroy(haven_squeue_t *q);

/*
	DYNAMIC QUUEUE (dqueue) note implemented yet
*/

/**
 * @brief dynamic queue data structure
 * 
 */
typedef struct	haven_dqueue_s {
	void		*data; // Dynamic byte buffer
	unsigned int	front;
	unsigned int	rear;
	unsigned int	size;
	unsigned int	data_size; // Use larger type (not limited to 255 bytes)
	unsigned int	capacity;
}	haven_dqueue_t;

/**
 * @brief initialize a dynamic queue
 * 
 * @param q 
 * @param data_size 
 */
void	haven_dqueue_init(haven_dqueue_t *q, unsigned int data_size);

/**
 * @brief 
 * 
 * @param q 
 * @param value 
 * @return int 
 */
int	haven_dqueue_push(haven_dqueue_t *q, const void *value);

/**
 * @brief 
 * 
 * @param q 
 * @param value 
 * @return int 
 */
int	haven_dqueue_pop(haven_dqueue_t *q, void *value);

/**
 * @brief 
 * 
 * @param q 
 */
void	haven_dqueue_destroy(haven_dqueue_t *q);

#endif