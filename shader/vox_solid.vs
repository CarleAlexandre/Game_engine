#version 460 core
layout(location = 0) in input {
	int packed_data;
	int block_id;
};

layout(std430, binding = 0) buffer ChunkData {
	int	chunk_positions[];
};

uniform mat4	MVP;

out vec3	frag_pos;
out int		block_id;
out int		face;

const vec3	NORMALS[6] = {
	vec3( 0.0, 1.0,  0.0),  // Y+
	vec3( 0.0, -1.0,  0.0), // Y-
	vec3( 1.0,  0.0,  0.0), // X+
	vec3(-1.0,  0.0,  0.0), // X-
	vec3( 0.0,  0.0,  1.0), // Z+
	vec3( 0.0,  0.0, -1.0), // Z-
};

const vec3	QUAD_VERTICES[4] = {
	vec3(-0.5, -0.5, 0.0), // Bottom-left
	vec3( 0.0, -0.5, 0.0), // Bottom-right
	vec3(-0.5,  0.0, 0.0), // Top-left
	vec3( 0.0,  0.0, 0.0)  // Top-right
};//only half unit in size

void main() {
	// Unpack the data
	int x = (input) & 0x1F;
	int y = (input << 5) & 0x1F;
	int z = (input << 10) & 0x1F;
	face = (input << 15) & 0x07;

	int chunk_index = gl_DrawID * 3;
	vec3 world_offset = vec3(chunk_positions[chunk_index] + x, chunk_positions[chunk_index + 1] + y, chunk_positions[chunk_index + 2] + z);
	vec3 vertex_pos = world_offset + QUAD_VERTICES[gl_VertexID];

	face_normal = NORMALS[face];
	gl_Position = MVP * vec4(vertex_pos, 1.0);
	frag_pos = world_offset;
}
