#version 460 core

flat in vec3	frag_pos;
flat in vec3	face_normal;
flat in int	block_id_out;
flat in int	face;
flat in int	extra;

out vec4 frag_color;

const vec3 color[6] = vec3[](
	vec3(0.5, 0.0, 0.0),
	vec3(0.0, 0.5, 0.0),
	vec3(0.2, 0.2, 0.5),
	vec3(0.5, 0.5, 0.0),
	vec3(0.5, 0.0, 0.5),
	vec3(0.0, 0.5, 0.5)
);

void main() {
	if (block_id_out == 1) {
		frag_color = vec4(color[face], 1.0);
	}
	if (block_id_out == 2) {
		frag_color = vec4(0.0, 0.0, 0.5, 0.5);
	}
}