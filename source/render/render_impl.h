#ifndef RENDER_IMPL_H
# define RENDER_IMPL_H

#include <stdlib.h>
#include <stdint.h>

#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>
#include <rcamera.h>
// #include "extern/rlights.h"

typedef struct Plane {
	Vector3 normal;
	float distance;
} Plane;
    
typedef struct Frustum {
	Plane planes[6];
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
	quad_data	faces[6][4096];
	unsigned int	faces_count[6];
}	voxel_mesh;

#endif
