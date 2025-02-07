#version 330 core

layout(location = 0) in int packed_data; // Packed vertex data
uniform vec3 world_pos;

out vec3 finalPos; // Unpacked position
out int face;      // Unpacked face
out int extra;     // Unpacked extra data

uniform mat4 matModel;
uniform mat4 matView;
uniform mat4 matProjection;

void main() {

	// Unpack the data
	int x = (packed_data >> 27) & 0x1F; // Extract x (5 bits)
	int y = (packed_data >> 22) & 0x1F; // Extract y (5 bits)
	int z = (packed_data >> 17) & 0x1F; // Extract z (5 bits)
	face = (packed_data >> 14) & 0x07;  // Extract face (3 bits)
	extra = packed_data & 0x3FFF;       // Extract extra data (14 bits)

	vec3 frag_pos = vec3(x, y, z);

	finalPos = frag_pos + world_pos;

	vec4 worldPosition = matModel * vec4(finalPos, 1.0);
	vec4 viewPosition = matView * worldPosition;
	vec4 clipPosition = matProjection * viewPosition;

	gl_Position = clipPosition;
}