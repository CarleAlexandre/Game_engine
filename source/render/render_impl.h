#ifndef RENDER_IMPL_H
# define RENDER_IMPL_H

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>
// #include <rcamera.h>
// #include "extern/rlights.h"

#define NUMBER_FACE 6

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

static const float	quad_vertices[] = {
	0.0f, 0.0f, 0.0f,//bot_left
	0.5f, 0.0f, 0.0f,//bot_right
	0.0f, 0.5f, 0.0f,//top_left
	0.5f, 0.5f, 0.0f,//top_right
};

static const uint32_t	quad_indices[] = {
	0,//bot_left
	1,//bot_right
	2,//top_left
	3,//top_right
};

#endif
