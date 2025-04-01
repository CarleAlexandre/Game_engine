#include "render_impl.h"

#define CHUNK_RESOLUTION 32
#define CHUNK_SIZE (CHUNK_RESOLUTION * CHUNK_RESOLUTION * CHUNK_RESOLUTION)
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

Texture2D	voxel_texture_create(void *data) {
	Image image = {	
		.data = data,
		.width = CHUNK_RESOLUTION,
		.height = CHUNK_RESOLUTION,
		.format = PIXELFORMAT_UNCOMPRESSED_R32,
		.mipmaps = 1,
	};

	Texture2D vox_texture = LoadTextureFromImage(image);
	assert(IsTextureValid(vox_texture));
	//now i can save and load it after chunk changed
}
