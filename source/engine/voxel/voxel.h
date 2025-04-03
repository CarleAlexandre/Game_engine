#ifndef VOXEL_H
# define VOXEL_H

#include <raylib.h>
#include <stdint.h>
#include "../type/octree.h"

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

// Helper function to get a chunk's voxel data from the arena
static inline voxel_data *get_chunk_voxels(voxel_world *world, int x, int y, int z) {
	return world->voxel_arena + ((x * CHUNK_STRIDE_X + y * CHUNK_STRIDE_Y + z) * CHUNK_SIZE);
}

#endif