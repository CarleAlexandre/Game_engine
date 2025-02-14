#version 460 core
layout(location = 0) in int packed_data;// Packed vertex data

uniform mat4 MVP;

layout(std430, binding = 0) buffer ChunkData {
	int chunk_positions[];
};

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
	int chunk_index = gl_DrawID * 3;
	ivec3 chunk_pos = ivec3(chunk_positions[chunk_index], chunk_positions[chunk_index + 1], chunk_positions[chunk_index + 2]);
	vec3 world_offset = local_pos + chunk_pos;
	frag_pos = world_offset;

	// Apply transformations
	gl_Position = MVP * vec4(world_offset, 1.0);
}
