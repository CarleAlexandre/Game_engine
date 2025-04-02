#include <raylib.h>
#include <stdint.h>

#define CHUNK_RESOLUTION 32
#define CHUNK_SIZE (CHUNK_RESOLUTION * CHUNK_RESOLUTION * CHUNK_RESOLUTION)
#define FACES 6

typedef struct chunk_texture {
	Texture2D texture[CHUNK_SIZE];
}	chunk_texture;

typedef struct voxel_data {
	uint16_t block_id;

}	voxel_data;

typedef struct voxel_chunk {
	voxel_data	*voxel[CHUNK_SIZE];//should use sparse voxel octree back
	Vector3		positon;
	uint32_t	bit[CHUNK_RESOLUTION * CHUNK_RESOLUTION];//used to send data to gpu
}	voxel_chunk;

typedef struct voxel_world {
	voxel_chunk	*chunk[CHUNK_SIZE];
}	voxel_world;


