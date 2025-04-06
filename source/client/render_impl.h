#ifndef RENDER_IMPL_H
# define RENDER_IMPL_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>

# define GRAY_VALUE(c) ((float)(c.r + c.g + c.b)/3.0f)

# define NUMBER_FACE 6

typedef struct Plane {
	Vector3 normal;
	float distance;
} Plane;
    
typedef struct Frustum {
	Plane planes[NUMBER_FACE];
} Frustum;

typedef  struct {
        uint32_t  count;
        uint32_t  instanceCount;
        uint32_t  firstIndex;
        uint32_t  baseVertex;
        uint32_t  baseInstance;
}	deic_t;

typedef enum {
	XN,
	XP,
	YN,
	YP,
	ZN,
	ZP,
}	direction_enum;

typedef struct	quad_data {
	bool		render;
	uint16_t	element;
	int		face_data;
}	quad_data;

typedef struct voxel_mesh {
	uint32_t	vao;
	uint32_t	vbo;
	uint32_t	ebo;
	uint32_t	ibo;
	quad_data	faces[NUMBER_FACE][4096];
	unsigned int	faces_count[NUMBER_FACE];
	Texture2D	face_texture;
}	voxel_mesh;

#endif
