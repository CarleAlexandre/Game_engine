#include <engine.h>
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

Vector3 get_vector_intersect(BoundingBox a, BoundingBox b) {
	Vector3 vec;

	return (vec);
}

bool	gjk_intersect() {

	return (false);
}

//down collision,
//up collision
//forward, backward, left, rigth
void collider(sv_player_t *player, Vector3 block_collision) {

} 

typedef struct s_hit_chunk { 
	Vector3 chunk_id;
	char posx, posy, posz;
}	hit_chunk_t;

typedef	struct s_chunk_hitbox {
	hit_chunk_t hit_chunk[8];
}	chunk_hitbox_t;

void check_chunk_for_collisions(sv_player_t *player, chunk_t *chunk) {
	// Assuming you have a function to get the chunk data
	if (!chunk) return;
    
	// Iterate over the blocks in the chunk that the bounding box intersects
	Vector3 min = Vector3Add(player->bound.min, player->pos);
	Vector3 max = Vector3Add(player->bound.max, player->pos);
    
	int block_min_x = (int)floor(min.x) % 32;
	int block_min_z = (int)floor(min.z) % 32;
	int block_min_y = (int)floor(min.y) % 32;
    
	int block_max_x = (int)floor(max.x) % 32;
	int block_max_z = (int)floor(max.z) % 32;
	int block_max_y = (int)floor(max.y) % 32;
    
	for (int x = block_min_x; x <= block_max_x; x++) {
	    for (int z = block_min_z; z <= block_max_z; z++) {
		for (int y = block_min_y; y <= block_max_y; y++) {
		    // Check if the block is solid
			// if (is_block_solid(chunk, x, y, z)) {
			// Handle collision
			//collider(player, (Vector3){});
		    //}
		}
	    }
	}
    }

void	get_player_block_pos(sv_player_t *player) {
	Vector3 min = Vector3Add(player->bound.min, player->pos);
	Vector3 max = Vector3Add(player->bound.max, player->pos);
	char flag = 8;

	Vector3 chunk_min = (Vector3){
		min.x / 32,
		min.z / 32,
		min.y / 32
	};
	Vector3 chunk_max = (Vector3){
		max.x / 32,
		max.z / 32,
		max.y / 32
	};

	chunk_t *chunk;

	// Iterate over the chunks that the bounding box intersects
	for (int x = chunk_min.x; x <= chunk_max.x; x++) {
		for (int z = chunk_min.z; z <= chunk_max.z; z++) {
			for (int y = chunk_min.y; y <= chunk_max.y; y++) {
			// Check for collisions in this chunk
				check_chunk_for_collisions(player, chunk);
			}
		}
	}
}

projectile_t shoot(Camera3D camera) {
	projectile_t bullet;
	bullet.shot = GetScreenToWorldRay((Vector2){(float)(GetScreenWidth() * 0.5), (float)(GetScreenWidth() * 0.5)}, camera);
}
