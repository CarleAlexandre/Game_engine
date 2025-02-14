#version 460 core
layout(location = 0) in int	packed_data;

layout(std430, binding = 0) buffer ChunkData {
	int	chunk_positions[];
};

uniform mat4	MVP;

out vec3	frag_pos;
out int		face;
out int		extra;

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
	vec3( 0.5, -0.5, 0.0), // Bottom-right
	vec3(-0.5,  0.5, 0.0), // Top-left
	vec3( 0.5,  0.5, 0.0)  // Top-right
};

void main() {
	// Unpack the data
	int x = (packed_data >> 27) & 0x1F;
	int y = (packed_data >> 22) & 0x1F;
	int z = (packed_data >> 17) & 0x1F;
	face = (packed_data >> 14) & 0x07;
	extra = (packed_data >> 11) & 0x07;


	vec3 face_normal = NORMALS[face];

	int chunk_index = gl_DrawID * 3;

	ivec3 chunk_pos = ivec3(chunk_positions[chunk_index], chunk_positions[chunk_index + 1], chunk_positions[chunk_index + 2]);
	
	vec3 world_offset = vec3(x, y, z) + chunk_pos;

	vec3 vertex_offset = QUAD_VERTICES[gl_VertexID];

	vec3 vertex_pos = world_offset + vertex_offset;

	gl_Position = MVP * vec4(vertex_pos, 1.0);

	frag_pos = world_offset;
	face_normal = face_normal;
	frag_extra = extra;
}
