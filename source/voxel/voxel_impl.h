#ifndef VOXEL_IMPL
# define VOXEL_IMPL

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include <raylib.h>
#include <raymath.h>
#include <rcamera.h>

#include "engine/core/type.h"

# define VOXEL_SIZE 0.5
# define CHUNK_SIZE 64

typedef enum {
	XN,
	XP,
	YN,
	YP,
	ZN,
	ZP,
}	direction_enum;

typedef struct	s_chunk_builder_data {
	uint64_t	face[CHUNK_SIZE];
	uint64_t	block_data[CHUNK_SIZE][CHUNK_SIZE];
}	chunk_builder_data_t;

typedef struct	s_face_data {
	int		block_id;
	int		face_data;
}	face_data_t;

typedef struct s_voxel {
	uint16_t	block_id;
	uint8_t		height;
	uint8_t		pressure;
}	voxel_t;

typedef  struct {
        uint32_t  count;
        uint32_t  instanceCount;
        uint32_t  firstIndex;
        uint32_t  baseVertex;
        uint32_t  baseInstance;
}	deic_t;

typedef struct s_chunk_mesh {
	uint32_t	vao;
	uint32_t	vbo;
	uint32_t	ebo;
	uint32_t	ibo;
	face_data_t	faces[6][4096];
	unsigned int	faces_count[6];
}	chunk_mesh_t;

typedef struct	s_chunk {
	haven_octree_t	*blocks;
	chunk_mesh_t	*mesh;
	bool		dirty_mesh;
	Vector3		pos;//center of min or max ?????
}	chunk_t;

typedef struct	s_world_render {
	uint32_t	vao;
	uint32_t	vbo;
	uint32_t	ebo;
	uint32_t	ibo;//used to store face data
	uint32_t	ssbo;//used to store chunk pos
}	world_render_t;

typedef struct s_world_mesh {
	world_render_t	render;
	face_data_t	*faces;
	unsigned int	faces_count;
	Vector3		*pos;
	unsigned int	pos_count;
}	world_mesh_t;

typedef struct	s_world {
	haven_octree_t	*chunks;
	Vector3		dim;

	world_mesh_t	mesh;
	unsigned int	rcount;
	chunk_t		*rqueue[512];
}	world_t;

#endif