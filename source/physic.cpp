#include <engine.hpp>
#include <raymath.h>

Vector3 apply_gravity(Vector3 pos, float mass) {
	Vector3 g = {1, mass * 0.2, 1};
	return (Vector3CrossProduct(g, pos));
}
