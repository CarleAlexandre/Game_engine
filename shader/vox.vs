#version 330 core
layout(location = 0) in int packed_data; // Packed vertex data
uniform vec3 world_pos;                  // World position of the chunk

uniform mat4 matModel;      // Model matrix
uniform mat4 matView;       // View matrix
uniform mat4 matProjection; // Projection matrix

out vec3 frag_pos;
out int face;
out int extra;

#define FACE_TOP    0
#define FACE_BOTTOM 1
#define FACE_LEFT   2
#define FACE_RIGHT  3
#define FACE_FRONT  4
#define FACE_BACK   5

void main() {
	// Unpack the data
	int x = (packed_data >> 27) & 0x1F;
	int y = (packed_data >> 22) & 0x1F;
	int z = (packed_data >> 17) & 0x1F;
	face = (packed_data >> 14) & 0x07;
	extra = (packed_data >> 11) & 0x07;

	// Convert to world coordinates
	vec3 local_pos = vec3(x, y, z);
	vec3 world_offset = local_pos + world_pos;
	frag_pos = world_offset;

	// Apply transformations
	if (face == FACE_TOP && extra == 2) {
		world_offset.y -= 0.1;
	}
	gl_Position = matProjection * matView * matModel * vec4(world_offset, 1.0);
}
