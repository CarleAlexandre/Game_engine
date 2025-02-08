#version 330 core
layout(location = 0) in int packed_data; // Packed vertex data
uniform vec3 world_pos;                  // World position of the chunk

uniform mat4 matModel;      // Model matrix
uniform mat4 matView;       // View matrix
uniform mat4 matProjection; // Projection matrix

out vec3 frag_pos;
out int face;
out int extra;

void main() {
	// Unpack the data
	int x = (packed_data >> 27) & 0x1F;
	int y = (packed_data >> 22) & 0x1F;
	int z = (packed_data >> 17) & 0x1F;
	face = (packed_data >> 14) & 0x07;
	extra = (packed_data >> 11) & 0x07;

	// Convert to world coordinates
	vec3 local_pos = vec3(x, y, z);
	vec3 world_offset = local_pos + (world_pos * 31.0);
	frag_pos = world_offset;

	// Apply transformations
	if (face == 0 && extra == 2) {
		world_offset.y -= 0.1;
	}
	gl_Position = matProjection * matView * matModel * vec4(world_offset, 1.0);
}
