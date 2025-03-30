#include "render_impl.h"

#define CHUNK_RESOLUTION 1024
#define CHUNK_SIZE (CHUNK_RESOLUTION * CHUNK_RESOLUTION * CHUNK_RESOLUTION)
#define SQRT_CHUNK 32768
#define CHUNK_WORLD_SIZE 64
#define FACES 6

typedef struct chunk_texture {
	Texture2D texture[CHUNK_SIZE];
}	chunk_texture;

typedef struct voxel_data {
	Color color;
}	voxel_data;

typedef struct voxel_chunk {
	voxel_data voxel[CHUNK_SIZE];
}	voxel_chunk;


void	voxel_texture_write() {

}

Texture2D	voxel_texture_create() {
	Texture2D texture = (Texture2D){
		.format = RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8,
		.width = CHUNK_RESOLUTION * CHUNK_RESOLUTION,
		.height = CHUNK_RESOLUTION,
		.id = 0,
		.mipmaps = 1,
	};

	texture.id = rlLoadTexture(0x00, SQRT_CHUNK, SQRT_CHUNK, RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8, 1);
	assert(IsTextureValid(texture));
	//now i can save and load it after chunk changed
}
