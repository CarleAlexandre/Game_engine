#include "render_impl.h"
#include <rcamera.h>

void	extract_frustum_from_matrix(const Matrix view, Frustum *frustum) {
	//stub
}

Frustum	frustum_from_camera(Camera camera) {
	Frustum frustum = {0};
	
	Matrix view = GetCameraViewMatrix(&camera);
	Matrix proj = GetCameraProjectionMatrix(&camera, GetScreenHeight() / GetScreenWidth());
	Matrix view_proj = MatrixMultiply(view, proj);
	extract_frustum_from_matrix(view_proj, &frustum);

	return (frustum);
}

float	plane_point_distance(const Plane plan, const Vector3 pos) {
    return (Vector3DotProduct(plan.normal, pos) + plan.distance);
}

bool	frustum_bounding_culling(BoundingBox box, Frustum frustum) {
	const int plan = 6;
	const int corner = 8;

	Vector3 corners[8] = {
		(Vector3){ box.min.x, box.min.y, box.min.z},
		(Vector3){ box.max.x, box.min.y, box.min.z},
		(Vector3){ box.min.x, box.max.y, box.min.z},
		(Vector3){ box.max.x, box.max.y, box.min.z},
		(Vector3){ box.min.x, box.min.y, box.max.z},
		(Vector3){ box.max.x, box.min.y, box.max.z},
		(Vector3){ box.min.x, box.max.y, box.max.z},
		(Vector3){ box.max.x, box.max.y, box.max.z}
	};

	int out = 0;
	for (int i = 0; i < plan; i++) {
		for (int j = 0; j < corner; j++) {
			if (plane_point_distance(frustum.planes[i], corners[j]) < 0) {
				out++;
			}
		}
	}
	if (out == corner) {
		return(false);
	}
	return (true);
}


Camera3D	camera_iso_init(void) {
	Camera3D camera;

	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;
	camera.position = (Vector3){40, 40, 40};
	camera.up = (Vector3){0, 1, 0};
	camera.target = (Vector3){0, 0, 0};

	return (camera);
}

void	camera_iso_update(Camera3D *camera, const Vector3 target) {
	camera->target = target;
	camera->position = Vector3AddValue(target, 40);
}