#include "render_impl.h"

Frustum	haven_frustum_get(Camera camera) {
	Frustum frustum;
	
	Matrix view = GetCameraViewMatrix(&camera);
	Matrix proj = GetCameraProjectionMatrix(&camera, GetScreenHeight() / GetScreenWidth());
	Matrix view_proj = MatrixMultiply(view, proj);
	extract_frustum_from_matrix(view_proj, &frustum);
	return (frustum);
}

bool	haven_frustum_visible(BoundingBox, Frustum frustum) {
	

	return ;
}
