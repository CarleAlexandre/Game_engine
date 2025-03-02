#include <engine.h>

void	extract_frustum_from_matrix(Matrix mat, Frustum *frustum) {
	// Left plane
	frustum->planes[0].normal.x = mat.m3 + mat.m0;
	frustum->planes[0].normal.y = mat.m7 + mat.m4;
	frustum->planes[0].normal.z = mat.m11 + mat.m8;
	frustum->planes[0].distance = mat.m15 + mat.m12;
    
	// Right plane
	frustum->planes[1].normal.x = mat.m3 - mat.m0;
	frustum->planes[1].normal.y = mat.m7 - mat.m4;
	frustum->planes[1].normal.z = mat.m11 - mat.m8;
	frustum->planes[1].distance = mat.m15 - mat.m12;
    
	// Bottom plane
	frustum->planes[2].normal.x = mat.m3 + mat.m1;
	frustum->planes[2].normal.y = mat.m7 + mat.m5;
	frustum->planes[2].normal.z = mat.m11 + mat.m9;
	frustum->planes[2].distance = mat.m15 + mat.m13;
    
	// Top plane
	frustum->planes[3].normal.x = mat.m3 - mat.m1;
	frustum->planes[3].normal.y = mat.m7 - mat.m5;
	frustum->planes[3].normal.z = mat.m11 - mat.m9;
	frustum->planes[3].distance = mat.m15 - mat.m13;
    
	// Near plane
	frustum->planes[4].normal.x = mat.m3 + mat.m2;
	frustum->planes[4].normal.y = mat.m7 + mat.m6;
	frustum->planes[4].normal.z = mat.m11 + mat.m10;
	frustum->planes[4].distance = mat.m15 + mat.m14;
    
	// Far plane
	frustum->planes[5].normal.x = mat.m3 - mat.m2;
	frustum->planes[5].normal.y = mat.m7 - mat.m6;
	frustum->planes[5].normal.z = mat.m11 - mat.m10;
	frustum->planes[5].distance = mat.m15 - mat.m14;
    
	// Normalize all planes
	for (int i = 0; i < 6; i++) {
		float length = Vector3Length(frustum->planes[i].normal);
		frustum->planes[i].normal = Vector3Normalize(frustum->planes[i].normal);
		frustum->planes[i].distance /= length;
	}
}

bool	is_box_in_frustum(BoundingBox box, Frustum frustum) {
	for (int i = 0; i < 6; i++) {
		Vector3 positive = box.min;
		
		if (frustum.planes[i].normal.x >= 0) positive.x = box.max.x;
		if (frustum.planes[i].normal.y >= 0) positive.y = box.max.y;
		if (frustum.planes[i].normal.z >= 0) positive.z = box.max.z;

		float distance = Vector3DotProduct(frustum.planes[i].normal, positive) + 
				frustum.planes[i].distance;
				
		if (distance < 0.01) return false;
	}
	return true;
}
