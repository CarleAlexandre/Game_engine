#include <engine.h>

void	load_chunk_data() {

}

void	unload_chunk_data() {

}

//voxel vertice : 

void	generate_chunk_mesh() {
	for (int x = 0; x < 32; x++) {
		for (int y = 0; y < 32; y++) {
			for (int z = 0; z < 32; z++) {
				//if empty don't add face;

				// if no vox above
				//add_top_face();
				
				// if no vox below
				//add_bottom_face();

				// if no vox on left
				//add_left_face();
				
				// if no vox on right
				//add_right_face();
				
				// if no vox behind
				//add_back_face();
				
				// if no vox in front
				//add_front_face();
			}
		}
	}
}

void	combine_chunk_data(){

}

void	draw_chunks() {

}
