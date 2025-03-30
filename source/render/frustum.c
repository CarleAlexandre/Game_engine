#include "render_impl.h"

Frustum	haven_frustum_get(Camera camera) {
	Frustum frustum;
	
	Matrix view = GetCameraViewMatrix(&camera);
	Matrix proj = GetCameraProjectionMatrix(&camera, GetScreenHeight() / GetScreenWidth());
	Matrix view_proj = MatrixMultiply(view, proj);
	extract_frustum_from_matrix(view_proj, &frustum);
	return (frustum);
}

float	haven_plane_point_distance(const Plane plan, const Vector3 pos) {
    return (Vector3DotProduct(plan.normal, pos) + plan.distance);
}

bool	haven_frustum_bound(BoundingBox box, Frustum frustum) {
	const int plan = 6;
	const int corner = 8;

	Vector3 corners[8] = {
		(Vector3){ box.min.x, box.min.y, box.min.z },
		(Vector3){ box.max.x, box.min.y, box.min.z },
		(Vector3){ box.min.x, box.max.y, box.min.z },
		(Vector3){ box.max.x, box.max.y, box.min.z },
		(Vector3){ box.min.x, box.min.y, box.max.z },
		(Vector3){ box.max.x, box.min.y, box.max.z },
		(Vector3){ box.min.x, box.max.y, box.max.z },
		(Vector3){ box.max.x, box.max.y, box.max.z }
	};

	int out = 0;
	for (int i = 0; i < plan; i++) {
		for (int j = 0; j < corner; j++) {
			if (haven_plane_point_distance(frustum.planes[i], corners[j]) < 0) {
				out++;
			}
		}
	}
	if (out == corner) {
		return(false);
	}
	return (true);
}
