#ifndef VOXEL_H
# define VOXEL_H

#include <raylib.h>
#include <stdint.h>
#include "../type/octree.h"
#include <rlgl.h>
#include <raymath.h>
#include <GL/gl.h>

#define CHUNK_RESOLUTION 32
#define CHUNK_SIZE (CHUNK_RESOLUTION * CHUNK_RESOLUTION * CHUNK_RESOLUTION)
#define FACES 6
#define WORLD_SIZE_X 32
#define WORLD_SIZE_Y 8
#define WORLD_SIZE_Z 32
#define CHUNK_STRIDE_Y (WORLD_SIZE_Z)
#define CHUNK_STRIDE_X (WORLD_SIZE_Y * WORLD_SIZE_Z)

typedef struct chunk_texture {
	Texture2D texture[CHUNK_SIZE];
}	chunk_texture;

typedef struct voxel_data {
	uint16_t block_id;
	uint8_t	material;
	
}	voxel_data;

typedef struct voxel_chunk {
	octree		voxel_tree;
	Vector3		positon;
	uint32_t	bit[CHUNK_RESOLUTION * CHUNK_RESOLUTION];//used to send data to gpu
}	voxel_chunk;

typedef struct voxel_world {
	voxel_chunk	*chunk[32][8][32];
	voxel_data	*voxel_arena;
}	voxel_world;

typedef struct voxel_mesh {
	int		*faces_buffer;
	uint32_t	face_count;
	// uint8_t		face_mask;//to filter face to push in render queue
}	voxel_mesh;

typedef  struct {
        uint32_t	count;
        uint32_t	instanceCount;
        uint32_t 	firstIndex;
        int		baseVertex;
        uint32_t	baseInstance;
}	DrawElementsIndirectCommand;

typedef struct voxel_chunk_render_queue {
	uint32_t			vao;
	uint32_t			vbo;
	uint32_t			ebo;
	//only buffer under will be updated above should be static
	uint32_t			ibo;//collection of all face to be rendered
	uint32_t			ssbo;
	
	uint32_t			face_count;
	
	uint32_t			chunk_count;
	
	DrawElementsIndirectCommand*	draw_cmd;//one draw cmd per chunk mesh
	
	void*				ssbo_data;// should be set at the same time of mesh sorting and updated every frame(should need to change if camera not updated)
	void*				ibo_data;//should countain all face to be rendered

	voxel_mesh*			meshes;//sort from farthest to nearest using player camera forward(could be easier said than done)
}	voxel_chunk_render_queue;

#endif