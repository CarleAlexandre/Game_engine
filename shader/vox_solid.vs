#version 430 core

# define FACE_YP	0
# define FACE_Y		1
# define FACE_XP	2
# define FACE_X		3
# define FACE_ZP	4
# define FACE_Z		5

layout(location = 0) in vec3	aPos;

layout(location = 1) in int	packed_data;
layout(location = 2) in int	block_id;

layout(std430, binding = 0) buffer ChunkData {
    vec3 chunk_positions[];
};

uniform mat4	mvp;

out vec3	frag_pos;
out vec3	face_normal;
out int		block_id_out;
out int		face;
out int		extra;

const vec3	NORMALS[6] = vec3[](
	vec3( 0.0,  1.0,  0.0), // top
	vec3( 0.0, -1.0,  0.0), // bottom
	vec3( 1.0,  0.0,  0.0), // right
	vec3(-1.0,  0.0,  0.0), // left
	vec3( 0.0,  0.0,  1.0), // front
	vec3( 0.0,  0.0, -1.0)  // back
);

void	main() {
	int x      = packed_data & 0x3F;
	int y      = (packed_data >> 6)  & 0x3F;
	int z      = (packed_data >> 12) & 0x3F;
	int height = (packed_data >> 18) & 0x3F;
	int width  = (packed_data >> 24) & 0x3F;

	face = block_id & 0x07;
	block_id_out = (block_id >> 3) & 0xFFFF;
	extra = (block_id >> 19) & 0xFF;

	vec3 basePos = aPos;

	if (face == FACE_YP) {
		basePos = vec3(aPos.x + 0.5, 1.0, aPos.y + 0.5);
	} else if (face == FACE_Y) {
		basePos = vec3(aPos.x + 0.5, 0.0, aPos.y + 0.5);
	} else if (face == FACE_XP) {
		basePos = vec3(1.0, aPos.y + 0.5, aPos.x + 0.5);
	} else if (face == FACE_X) {
		basePos = vec3(0.0, aPos.y + 0.5, aPos.x + 0.5);
	} else if (face == FACE_ZP) {
		basePos = vec3(aPos.x + 0.5, aPos.y + 0.5, 1.0);
	} else if (face == FACE_Z) {
		basePos = vec3(aPos.x + 0.5, aPos.y + 0.5, 0.0);
	}

	vec3 voxel_offset = vec3(x, y, z) * 0.5;
	vec3 chunk_offset = chunk_positions[gl_DrawID] + voxel_offset;

	vec3 vertex_pos = aPos + chunk_offset;

	frag_pos = vertex_pos;
	face_normal = NORMALS[face];
	gl_Position = mvp * vec4(vertex_pos, 1.0);
}