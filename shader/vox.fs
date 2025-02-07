#version 330 core

out vec4 frag_color;

// in vec3 frag_pos;
flat in int face;

void main() {
	// vec3 NORMALS[6] = {
	// 	vec3( 0.0,  1.0,  0.0),
	// 	vec3( 0.0, -1.0,  0.0),
	// 	vec3( 1.0,  0.0,  0.0),
	// 	vec3(-1.0,  1.0,  0.0),
	// 	vec3( 0.0,  0.0,  1.0),
	// 	vec3( 0.0,  0.0, -1.0)
	// };
	// vec3 normal = NORMALS[face];
	vec3 color = vec3(0.0);
	if (face == 0) color = vec3(1.0, 0.0, 0.0); // Top face (red)
	else if (face == 1) color = vec3(0.0, 1.0, 0.0); // Bottom face (green)
	else if (face == 2) color = vec3(0.0, 0.0, 1.0); // Left face (blue)
	else if (face == 3) color = vec3(1.0, 1.0, 0.0); // Right face (yellow)
	else if (face == 4) color = vec3(1.0, 0.0, 1.0); // Front face (magenta)
	else if (face == 5) color = vec3(0.0, 1.0, 1.0); // Back face (cyan)

	frag_color = vec4(color, 1.0);
}