#version 460 core

flat in vec3	frag_pos;
flat in vec3	face_normal;
flat in int	block_id_out;
flat in int	face;
flat in int	extra;

out vec4 frag_color;

void main() {
	if (block_id_out == 1) {
		vec3 color = vec3(0.0);
		if (face == 0) color = vec3(0.5, 0.0, 0.0);
		else if (face == 1) color = vec3(0.0, 0.5, 0.0);
		else if (face == 2) color = vec3(0.2, 0.2, 0.5);
		else if (face == 3) color = vec3(0.5, 0.5, 0.0);
		else if (face == 4) color = vec3(0.5, 0.0, 0.5);
		else if (face == 5) color = vec3(0.0, 0.5, 0.5);
		frag_color = vec4(color, 1.0);
	}
	if (block_id_out == 2) {
		frag_color = vec4(0.0, 0.0, 0.5, 0.5);
	}
}