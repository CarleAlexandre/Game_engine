#version 330 core
layout(location = 0) in int packed_data; // Packed vertex data
uniform vec3 world_pos;

out vec3 frag_pos; // Unpacked position
out int face;      // Unpacked face
out int extra;     // Unpacked extra data

void main() {
	vec3 NORMALS[6] = {
		vec3( 0.0,  1.0,  0.0),
		vec3( 0.0, -1.0,  0.0),
		vec3( 1.0,  0.0,  0.0),
		vec3(-1.0,  1.0,  0.0),
		vec3( 0.0,  0.0,  1.0),
		vec3( 0.0,  0.0, -1.0)
	};

	// Unpack the data
	int x = (packed_data >> 27) & 0x1F; // Extract x (5 bits)
	int y = (packed_data >> 22) & 0x1F; // Extract y (5 bits)
	int z = (packed_data >> 17) & 0x1F; // Extract z (5 bits)
	face = (packed_data >> 14) & 0x07;  // Extract face (3 bits)
	extra = packed_data & 0x3FFF;       // Extract extra data (14 bits)

	// Convert to world coordinates (optional)
	frag_pos = vec3(x, y, z);
	vec3 normal = NORMALS[face];

	// Transform the position for rendering
	vec3 finalPos = vertex_pos + world_pos;
	gl_Position = vec4(frag_pos, 1.0); // Replace with your projection/view/model matrix
}