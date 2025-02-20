#include <prototype.h>
#include <ext/Frustum.h>

face_data_t	pack_face_data(unsigned char pos[3], unsigned char face, unsigned char height, unsigned char width, unsigned short id) {
	face_data_t data;

	data.block_id = (id << 3 | face);
	data.face_data = ((height << 24) | (width << 18) | (pos[2] << 12) | (pos[1] << 6) | pos[0]);
	return (data);
}

// each world part are 2048 vox^3 or 64 chunk ^ 3 but it's an octree

// only part that are heavily player edited will be save and kept in memory else it will regenerate that will make use save lot of disk space

//'also dungeon should not be kept but genereted when entered and cleared when outside (else i had mecanics for player to go back inside)

//gen chunk mesh

face_data_t	add_face_to_mesh(int x, int y, int z, int face, int id, char width, char height) {
	char pos[3];

	pos[0] = x;
	pos[1] = y;
	pos[2] = z;

	return (pack_face_data(pos, face, height, width, id));
}

void	greedy_mesh_face(chunk_t *chunk, dyn_array_t *mesh, int face) {

}

void	mesh_face(chunk_t *chunk, dyn_array_t *mesh, int face) {
	for (int x = 0; x < 64; x++) {
		for (int z = 0; z < 64; z++) {
			for (int y = 0; y < 64; y++) {
				svo_node_t *node = svo_get_node((Vector3){x, y, z}, chunk->blocks);
				if (!node || !node->data) {
					continue;
				}

				voxel_t *vox = (voxel_t *)node->data;

				switch(face) {
					case (FACE_TOP):
						if (y == 63 || !svo_get_node((Vector3){x, y + 1, z}, chunk->blocks)) {
							face_data_t top_face = add_face_to_mesh(x, y, z, FACE_TOP, vox->block_id, 1, 1);
							dyn_add_elem(mesh, &top_face);
						}
					break;
					case (FACE_BOTTOM):
						if (y == 0 || !svo_get_node((Vector3){x, y - 1, z}, chunk->blocks)) {
							face_data_t bottom_face = add_face_to_mesh(x, y, z, FACE_BOTTOM, vox->block_id, 1, 1);
							dyn_add_elem(mesh, &bottom_face);
						}
					break;
					case (FACE_RIGHT):
						if (x == 63 || !svo_get_node((Vector3){x + 1, y, z}, chunk->blocks)) {
							face_data_t right_face = add_face_to_mesh(x, y, z, FACE_RIGHT, vox->block_id, 1, 1);
							dyn_add_elem(mesh, &right_face);
						}
					break;
					case (FACE_LEFT):
						if (x == 0 || !svo_get_node((Vector3){x - 1, y, z}, chunk->blocks)) {
							face_data_t left_face = add_face_to_mesh(x, y, z, FACE_LEFT, vox->block_id, 1, 1);
							dyn_add_elem(mesh, &left_face);
						}
					break;
					case (FACE_FRONT):
						if (z == 63 || !svo_get_node((Vector3){x, y, z + 1}, chunk->blocks)) {
							face_data_t front_face = add_face_to_mesh(x, y, z, FACE_FRONT, vox->block_id, 1, 1);
							dyn_add_elem(mesh, &front_face);
						}
					break;
					case (FACE_BACK):
						if (z == 0 || !svo_get_node((Vector3){x, y, z - 1}, chunk->blocks)) {
							face_data_t back_face = add_face_to_mesh(x, y, z, FACE_BACK, vox->block_id, 1, 1);
							dyn_add_elem(mesh, &back_face);
						}
					break;
				}
			}
		}
	}
}

static inline int	num_cmp(int f1, int f2) {
	if (f1 > f2) {
		return (1);
	} else if (f1 < f2) {
		return (2);
	}
	return (0);
}

chunk_render_t	*generate_chunk_mesh(chunk_t *chunk, dyn_array_t *mesh, Vector3 pos) {
	chunk_render_t	*render = malloc(sizeof(chunk_render_t));
	assert(render);

	char	face[3][2] = {{FACE_LEFT, FACE_TOP}, {FACE_BOTTOM, FACE_TOP}, {FACE_BACK, FACE_FRONT}};
	int	c_pos[3] = {chunk->bounding_box.min.x, chunk->bounding_box.min.y, chunk->bounding_box.min.z}, p_pos[3];
	p_pos[0] = pos.x;
	p_pos[1] = pos.y;
	p_pos[2] = pos.z;

	render->face_offset = mesh->size;
	for (int i = 0; i < 6; i++) {
		switch(num_cmp(c_pos[i], p_pos[i])) {
			case (0):
				mesh_face(chunk, mesh, face[i][0]);
				break;
			case (1):
				mesh_face(chunk, mesh, face[i][1]);
				break;
			case (2):
				mesh_face(chunk, mesh, face[i][0]);
				mesh_face(chunk, mesh, face[i][1]);
				break;
		}
	}
	render->face_count = mesh->size - render->face_offset;
	render->x = chunk->bounding_box.min.x;
	render->y = chunk->bounding_box.min.y;
	render->z = chunk->bounding_box.min.z;
	return (render);
}

//World Render

bool	is_chunk_visible(BoundingBox *bbox, Frustum *frustum) {
	return (AABBoxInFrustum(frustum, bbox->min, bbox->max));
}

chunk_t* get_chunk_at_position(svo_t *chunks, Vector3 position) {
	Vector3 chk;
	chk.x = (int)(position.x / 32);
	chk.y = (int)(position.y / 32);
	chk.z = (int)(position.z / 32);
 
	svo_node_t *node = svo_get_node(chk, chunks);
	if (!node) {
		return (0x00);
	}

	return (node->data);// need to get chunk pos
}
    
voxel_t* get_voxel_at_position(chunk_t *chunk, Vector3 position) {
	if (!chunk)
		return(NULL);
    
	Vector3 vox;

	vox.x = (int)(position.x * 2.0f) % 32;
	vox.y = (int)(position.y * 2.0f) % 32;
	vox.z = (int)(position.z * 2.0f) % 32;
    
	return (svo_get_node(vox, chunk->blocks)->data);
}

//using dda
bool raycast(Ray ray, svo_t *tree) {
	Vector3 ray_pos = ray.position;
	Vector3 ray_dir = Vector3Normalize(ray.direction);
    
	Vector3 step = {
		(ray_dir.x > 0) ? 0.5f : -0.5f,
		(ray_dir.y > 0) ? 0.5f : -0.5f,
		(ray_dir.z > 0) ? 0.5f : -0.5f
	};

	Vector3 delta_dist = {
		fabsf(0.5f / ray_dir.x),
		fabsf(0.5f / ray_dir.y),
		fabsf(9.5f / ray_dir.z)
	};

	Vector3 side_dist;
	side_dist.x = (ray_dir.x > 0) ? (ceilf(ray_pos.x) - ray_pos.x) : (ray_pos.x - floorf(ray_pos.x));
	side_dist.y = (ray_dir.y > 0) ? (ceilf(ray_pos.y) - ray_pos.y) : (ray_pos.y - floorf(ray_pos.y));
	side_dist.z = (ray_dir.z > 0) ? (ceilf(ray_pos.z) - ray_pos.z) : (ray_pos.z - floorf(ray_pos.z));
	side_dist = Vector3Multiply(side_dist, delta_dist);

	while (true) {
		svo_node_t *node = svo_get_node(ray_pos, tree);
		chunk_t *current_chunk = (node == NULL ? NULL : node->data);
		if (current_chunk) {
			voxel_t *current_voxel = get_voxel_at_position(current_chunk, ray_pos);
			if (current_voxel && current_voxel->block_id != 0) {
				return(true);
			}
		}

		if (side_dist.x < side_dist.y && side_dist.x < side_dist.z) {
			ray_pos.x += step.x;
			side_dist.x += delta_dist.x;
		} else if (side_dist.y < side_dist.z) {
			ray_pos.y += step.y;
			side_dist.y += delta_dist.y;
		} else {
			ray_pos.z += step.z;
			side_dist.z += delta_dist.z;
		}

		float max_distance = 1024.0f;//need to chgeck if it's good or not render distance
		if (Vector3Distance(ray.position, ray_pos) > max_distance) {
			return(false);
		}
	}
	return(false);
}

bool	is_chunk_occluded(chunk_t *chunk, Vector3 camera_position, svo_t *chunks) {
	Vector3	chunk_center;
	Ray	ray;

	chunk_center = Vector3Add(chunk->bounding_box.min, chunk->bounding_box.max);
	chunk_center = Vector3Divide(chunk_center, (Vector3){0.5, 0.5, 0.5});	
	ray.direction = Vector3Normalize(Vector3Subtract(chunk_center, camera_position));
	ray.position = camera_position;
	return (raycast(ray, chunks));
}

void	gen_render_chunk(world_t *world, engine_t *engine) {
	Vector3 forward = GetCameraForward(&engine->camera);
	if (engine->render.world.rqueue) {
		dyn_clear(engine->render.world.rqueue);
	}

	Frustum frustum;
	ExtractFrustum(&frustum);

	static bool only_once = false;

	for (int x = 0; x < 4; x++) {
		for (int z = 0; z < 4; z++) {
			for (int y = 0; y < 4; y++) {
				svo_node_t *node = svo_get_node((Vector3){x,y,z}, world->tree);
				if (node && node->data) {
					chunk_t *current_chunk = node->data;
					if (is_chunk_visible(&current_chunk->bounding_box, &frustum)){// && !is_chunk_occluded(current_chunk, engine->camera.position, world->tree)) {
						printf("current_chunk: %.0f, %.0f, %.0f\n", current_chunk->bounding_box.min.x, current_chunk->bounding_box.min.y, current_chunk->bounding_box.min.z);
						printf("current_chunk: %.0f, %.0f, %.0f\n", current_chunk->bounding_box.max.x, current_chunk->bounding_box.max.y, current_chunk->bounding_box.max.z);
						printf("current_chunk: %i, %i, %i\n", x, y, z);
						chunk_render_t *rend = generate_chunk_mesh(current_chunk, engine->render.world.faces, engine->camera.position);
						dyn_add_elem(engine->render.world.rqueue, rend);
					}
				}
			}
		}
	}
}

int chunk_render_comparator(const void *a, const void *b) {
	chunk_render_t *chunk_a = (chunk_render_t *)a;
	chunk_render_t *chunk_b = (chunk_render_t *)b;

	if (chunk_a->x != chunk_b->x) return chunk_a->x - chunk_b->x;
	if (chunk_a->y != chunk_b->y) return chunk_a->y - chunk_b->y;
	return chunk_a->z - chunk_b->z;
}

void	reload_voxel_world(world_t *world, engine_t *engine) {
	gen_render_chunk(world, engine);
	reload_world_render(&engine->render);
}
