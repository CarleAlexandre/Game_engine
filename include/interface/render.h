#ifndef RENDER_H
# define RENDER_H

# include <engine.h>

/*
*/
void	extract_frustum_from_matrix(Matrix mat, Frustum *frustum);

/*
*/
bool	is_box_in_frustum(BoundingBox box, Frustum frustum);

/*
*/
gbuffer_t	load_gbuffer(int width, int height, unsigned int shader_id);


// VOXEL




#endif
 