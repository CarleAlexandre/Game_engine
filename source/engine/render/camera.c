#include "render_impl.h"
#include <rcamera.h>

void	extract_frustum_from_matrix(const Matrix vp, Frustum *frustum, bool normalize) {
	// Left Plane
	frustum->planes[0].normal.x = vp.m8  + vp.m1;
	frustum->planes[0].normal.y = vp.m9  + vp.m2;
	frustum->planes[0].normal.z = vp.m10 + vp.m3;
	frustum->planes[0].distance = vp.m11 + vp.m4;
	
	// Right Plane
	frustum->planes[1].normal.x = vp.m8  - vp.m1;
	frustum->planes[1].normal.y = vp.m9  - vp.m2;
	frustum->planes[1].normal.z = vp.m10 - vp.m3;
	frustum->planes[1].distance = vp.m11 - vp.m4;
	
	// Bottom Plane
	frustum->planes[2].normal.x = vp.m8  + vp.m5;
	frustum->planes[2].normal.y = vp.m9  + vp.m6;
	frustum->planes[2].normal.z = vp.m10 + vp.m7;
	frustum->planes[2].distance = vp.m11 + vp.m8;
	
	// Top Plane
	frustum->planes[3].normal.x = vp.m8  - vp.m5;
	frustum->planes[3].normal.y = vp.m9  - vp.m6;
	frustum->planes[3].normal.z = vp.m10 - vp.m7;
	frustum->planes[3].distance = vp.m11 - vp.m8;
	
	// Near Plane
	frustum->planes[4].normal.x = vp.m8  + vp.m9;
	frustum->planes[4].normal.y = vp.m9  + vp.m10;
	frustum->planes[4].normal.z = vp.m10 + vp.m11;
	frustum->planes[4].distance = vp.m11 + vp.m12;
	
	// Far Plane
	frustum->planes[5].normal.x = vp.m8  - vp.m9;
	frustum->planes[5].normal.y = vp.m9  - vp.m10;
	frustum->planes[5].normal.z = vp.m10 - vp.m11;
	frustum->planes[5].distance = vp.m11 - vp.m12;
	
		// Normalize all the planes if requested
	if (normalize) {
		for (int i = 0; i < NUMBER_FACE; i++) {
			float length = sqrtf(
				frustum->planes[i].normal.x * frustum->planes[i].normal.x +
				frustum->planes[i].normal.y * frustum->planes[i].normal.y +
				frustum->planes[i].normal.z * frustum->planes[i].normal.z
			);
	    
			if (length != 0.0f) {
				frustum->planes[i].normal.x /= length;
				frustum->planes[i].normal.y /= length;
				frustum->planes[i].normal.z /= length;
				frustum->planes[i].distance /= length;
			}
		}
	}
}

Frustum	frustum_from_camera(Camera camera) {
	Frustum frustum = {0};
	
	Matrix view = GetCameraViewMatrix(&camera);
	Matrix proj = GetCameraProjectionMatrix(&camera, GetScreenHeight() / GetScreenWidth());
	Matrix view_proj = MatrixMultiply(view, proj);
	extract_frustum_from_matrix(view_proj, &frustum, false);

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