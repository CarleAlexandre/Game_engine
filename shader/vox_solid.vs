#version 460 core

layout(location = 0) in vec3 aPos;

layout(location = 1) in int packed_data; // Packed face data
layout(location = 2) in int block_id;   // Block ID

layout(std430, binding = 0) buffer ChunkData {
    int chunk_positions[]; // Array of chunk positions (x, y, z)
};

uniform mat4 MVP;

out vec3 frag_pos;
out vec3 face_normal;
out int block_id_out;
out int face;
out int extra;

const vec3 NORMALS[6] = vec3[](
    vec3( 0.0,  1.0,  0.0), // Y+
    vec3( 0.0, -1.0,  0.0), // Y-
    vec3( 1.0,  0.0,  0.0), // X+
    vec3(-1.0,  0.0,  0.0), // X-
    vec3( 0.0,  0.0,  1.0), // Z+
    vec3( 0.0,  0.0, -1.0)  // Z-
);

mat4	get_rotation_matrix(vec3 norm) {
	vec3 up = vec3(0.0, 1.0, 0.0);

	if (abs(norm.y) > 0.99) {
		up = vec3(0.0, 0.0, 1.0);
	}

	vec3 right = normalize(cross(up, norm));
	up = normalize(cross(norm, right));

	return (
		mat4(
			vec4(right, 0.0),
			vec4(up, 0.0),
			vec4(norm, 0.0),
			vec4(0.0, 0.0, 0.0, 1.0)
		)
	);
}

void	main() {
	int x = (packed_data) & 0x3F;
	int y = (packed_data >> 6) & 0x3F;
	int z = (packed_data >> 12) & 0x3F;
	int height = (packed_data >> 18) & 0x3F;
	int width = (packed_data >> 24) & 0x3F;
	face = min((block_id) & 0x07, 5);
	block_id_out = (block_id >> 3) & 0xFFFF;
	extra = (block_id >> 19) & 0xFF;

	int chunk_index = gl_DrawID * 3;
	vec3 scaled_chunk_pos = vec3(chunk_positions[chunk_index], chunk_positions[chunk_index + 1], chunk_positions[chunk_index + 2]) * 32.0;

	vec3 voxel_offset = vec3(x, y, z) * 0.5;
	vec3 world_offset = scaled_chunk_pos + voxel_offset;

	face_normal = NORMALS[face];
	mat4 rot = get_rotation_matrix(face_normal);

	vec3 vertex_pos = world_offset + (rot * vec4(aPos, 1.0)).xyz;

	// gl_Position = MVP * vec4(vertex_pos, 1.0);
	gl_Position = MVP * vec4(aPos + world_offset, 1.0);
	frag_pos = world_offset;
}