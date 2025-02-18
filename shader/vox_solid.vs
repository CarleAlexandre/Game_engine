#version 460 core

layout(location = 0) in int input_data;
layout(location = 1) in int block_id;

layout(std430, binding = 0) buffer ChunkData {
	int	chunk_positions[];
};

uniform mat4	MVP;

out vec3	frag_pos;
out vec3	face_normal;
out int		block_id_out;
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
	vec3(-0.25, -0.25, 0.0), // Bottom-left
	vec3( 0.25, -0.25, 0.0), // Bottom-right
	vec3(-0.25,  0.25, 0.0), // Top-left
	vec3( 0.25,  0.25, 0.0)  // Top-right
};//only half unit in size


void main() {
	// Unpack the data
	int x = (input_data) & 0x3f;
	int y = (input_data >> 6) & 0x3f;
	int z = (input_data >> 12) & 0x3f;
	int height = (input_data >> 18) & 0x3f;
	int width = (input_data >> 24) & 0x3f;
	face = (block_id) & 0x07;
	block_id_out = (block_id >> 3) & 0xffff;
	extra = (block_id >> 19) & 0xff;

	int chunk_index = gl_DrawID * 3;//if doesn't work use either gl_InstanceID (if gl version is too low)
	vec3 scaled_chunk_pos = vec3(chunk_positions[chunk_index], chunk_positions[chunk_index + 1], chunk_positions[chunk_index + 2]) * 32.0;
	vec3 voxel_offset = vec3(x, y, z) * 0.5;
	vec3 world_offset = scaled_chunk_pos + voxel_offset;
	vec3 vertex_pos = world_offset + QUAD_VERTICES[gl_VertexID];

	face_normal = NORMALS[face];
	gl_Position = MVP * vec4(vertex_pos, 1.0);
	frag_pos = world_offset;
}
