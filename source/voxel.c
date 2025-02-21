#include <prototype.h>

face_data_t	pack_face_data(unsigned char pos[3], unsigned char face, unsigned char height, unsigned char width, unsigned short id) {
	face_data_t data;

	data.block_id = (id << 3 | face);
	data.face_data = ((height << 24) | (width << 18) | (pos[2] << 12) | (pos[1] << 6) | pos[0]);
	return (data);
}

//World Render

chunk_render_t	gen_chunk_mesh(chunk_t *current, const chunk_t *neighbor[6]) {
	chunk_render_t mesh;
	mesh.faces = init_dyn_array(sizeof(face_data_t));
	for (char x = 0; x < 64; x++) {
		for (char z = 0; z < 64; z++) {
			for (char y = 0; y < 64; y++) {
				void *node = svo_get_node((Vector3){x, y, z}, current->blocks);
				if (node) continue;
				if (y == 63 && (neighbor[FACE_YP] && !svo_get_node((Vector3){x, y, z}, neighbor[FACE_YP]->blocks) || !neighbor[FACE_YP])
				|| !svo_get_node((Vector3){x, 0, z}, current->blocks)) {
					face_data_t face = pack_face_data((char *){x, y, z}, FACE_YP, 1, 1, 1);
					dyn_add_elem(mesh.faces, (void *)&face);
						
				}
				if (y == 0 && (neighbor[FACE_Y] && !svo_get_node((Vector3){x, y, z}, neighbor[FACE_Y]->blocks) || !neighbor[FACE_Y])
				|| !svo_get_node((Vector3){x, 63, z}, current->blocks)) {
					face_data_t face = pack_face_data((char *){x, y, z}, FACE_Y, 1, 1, 1);
					dyn_add_elem(mesh.faces, (void *)&face);

				}
				if (x == 63 && (neighbor[FACE_XP] && !svo_get_node((Vector3){x, y, z}, neighbor[FACE_XP]->blocks) || !neighbor[FACE_XP])
				|| !svo_get_node((Vector3){0, y, z}, current->blocks)) {
					face_data_t face = pack_face_data((char *){x, y, z}, FACE_XP, 1, 1, 1);
					dyn_add_elem(mesh.faces, (void *)&face);
				}
				if (x == 0 && (neighbor[FACE_X] && !svo_get_node((Vector3){x, y, z}, neighbor[FACE_X]->blocks) || !neighbor[FACE_X])
				|| !svo_get_node((Vector3){63, y, z}, current->blocks)) {
					face_data_t face = pack_face_data((char *){x, y, z}, FACE_X, 1, 1, 1);
					dyn_add_elem(mesh.faces, (void *)&face);
				}
				if (z == 63 && (neighbor[FACE_ZP] && !svo_get_node((Vector3){x, y, z}, neighbor[FACE_ZP]->blocks) || !neighbor[FACE_ZP])
				|| !svo_get_node((Vector3){x, y, 0}, current->blocks)) {
					face_data_t face = pack_face_data((char *){x, y, z}, FACE_ZP, 1, 1, 1);
					dyn_add_elem(mesh.faces, (void *)&face);
				}
				if (z == 0 && (neighbor[FACE_Z] && !svo_get_node((Vector3){x, y, z}, neighbor[FACE_Z]->blocks) || !neighbor[FACE_Z])
				|| !svo_get_node((Vector3){x, y, 63}, current->blocks)) {
					face_data_t face = pack_face_data((char *){x, y, z}, FACE_Z, 1, 1, 1);
					dyn_add_elem(mesh.faces, (void *)&face);
				}
			}
		}
	}
}

const chunk_t *get_chunk_neighbor(Vector3 pos, svo_t *tree) {
	float nx, ny, nz;

	nx = pos.x;
	ny = pos.y;
	nz = pos.z;

	chunk_t *chunk[6];
	chunk[0] = svo_get_node((Vector3){nx, ny, nz}, tree);
	chunk[1] = svo_get_node((Vector3){nx, ny, nz}, tree);
	chunk[2] = svo_get_node((Vector3){nx, ny, nz}, tree);
	chunk[3] = svo_get_node((Vector3){nx, ny, nz}, tree);
	chunk[4] = svo_get_node((Vector3){nx, ny, nz}, tree);
	chunk[5] = svo_get_node((Vector3){nx, ny, nz}, tree);
	return (chunk);
}

void	gen_world_mesh(world_t *world, engine_t *engine) {
	for (int x = 0; x < 4; x++) {
		for (int z = 0; z < 4; z++) {
			for (int y = 0; y < 4; y++) {
				svo_node_t *node = svo_get_node((Vector3){x,y,z}, world->tree);
				if (node && node->data) {
					chunk_t *current_chunk = node->data;
					current_chunk->mesh = gen_chunk_mesh(current_chunk, get_chunk_neighbor((Vector3){x,y,z}, world->tree));
				}
			}
		}
	}
}

//use this whenever a chunk need to have it's mesh updated
void	update_chunk_render(world_t *world, Vector3 pos) {
	svo_node_t *node = svo_get_node(pos, world->tree);
	if (node && node->data) {
		chunk_t *current_chunk = node->data;
		destroy_dyn_array(current_chunk->mesh.faces);
		current_chunk->mesh = gen_chunk_mesh(current_chunk, get_chunk_neighbor(pos, world->tree));
	}
}

bool	is_chunk_visible(BoundingBox *bbox, Vector3 pos) {
	return(true);
}

//check this every few frame;
void	update_world_render(world_t *world, engine_t *engine) {
	if (!world->render.render_queue) {
		world->render.render_queue = init_dyn_array(sizeof(void *));
	}

	for (int x = 0; x < 4; x++) {
		for (int z = 0; z < 4; z++) {
			for (int y = 0; y < 4; y++) {
				svo_node_t *node = svo_get_node((Vector3){x,y,z}, world->tree);
				if (node && node->data) {
					chunk_t *current_chunk = node->data;
					if (is_chunk_visible(&current_chunk->bounding_box, engine->camera.position)) {
						dyn_add_elem(world->render.render_queue, &current_chunk);
					}
				}
			}
		}
	}
}

int chunk_render_comparator(const void *a, const void *b) {
	chunk_render_t *chunk_a = (chunk_render_t *)a;
	chunk_render_t *chunk_b = (chunk_render_t *)b;

	if (chunk_a->position.x != chunk_b->position.x) return chunk_a->position.x - chunk_b->position.x;
	if (chunk_a->position.y != chunk_b->position.y) return chunk_a->position.y - chunk_b->position.y;
	return chunk_a->position.z - chunk_b->position.z;
}
