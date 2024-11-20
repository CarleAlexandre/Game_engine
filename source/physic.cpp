#include <engine.hpp>
#include <raymath.h>

Vector3 apply_gravity(Vector3 vel, float mass) {
	vel.y +=  -9.8;// only if not on ground ::
	return (vel);
}

bool bound_intersect(BoundingBox a, BoundingBox b) {
	return (a.min.x <= b.max.x &&
		a.max.x >= b.min.x &&
		a.min.y <= b.max.y &&
		a.max.y >= b.min.y &&
		a.min.z <= b.max.z &&
		a.max.z >= b.min.z);
}

bool point_intersect(Vector3 point, BoundingBox box) {
	return (point.x >= box.min.x &&
		point.x <= box.max.x &&
		point.y >= box.min.y &&
		point.y <= box.max.y &&
		point.z >= box.min.z &&
		point.z <= box.max.z);
}

void collider(level_t level, sv_player_t player) {
	if (bound_intersect(level.terrain.bound, player.bound)) {
	}
	for (auto obj : level.objs) {
		if (bound_intersect(player.bound, obj.bound)) {

		}
	}
} 
