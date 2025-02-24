#version 430 core

#define FACE_YP    0
#define FACE_Y     1
#define FACE_XP    2
#define FACE_X     3
#define FACE_ZP    4
#define FACE_Z     5

layout(location = 0) in vec3	aPos;
layout(location = 1) in int	packed_data;
layout(location = 2) in int	block_id;

uniform mat4	mvp;
uniform mat4	matView;
uniform mat4	matProjection;
uniform vec3	chunk_pos;
uniform mat4	matModel; 

out vec3 frag_pos;
out vec3 face_normal;
flat out int block_id_out;
flat out int face;

const vec3 NORMALS[6] = vec3[](
	vec3( 0.0,  1.0,  0.0), // y+
	vec3( 0.0, -1.0,  0.0), // y-
	vec3( 1.0,  0.0,  0.0), // x+
	vec3(-1.0,  0.0,  0.0), // x-
	vec3( 0.0,  0.0,  1.0), // z+
	vec3( 0.0,  0.0, -1.0)  // z-
);

// Corrected rotation matrices
const mat3 ROTATIONS[6] = mat3[](
	/* FACE_YP (+Y) */ mat3(1,0,0, 0,0,-1, 0,1,0),    // Rotate from +Z to +Y
	/* FACE_Y  (-Y) */ mat3(1,0,0, 0,0,1, 0,-1,0),    // Rotate from +Z to -Y
	/* FACE_XP (+X) */ mat3(0,0,-1, 0,1,0, 1,0,0),    // Rotate from +Z to +X
	/* FACE_X  (-X) */ mat3(0,0,1, 0,1,0, -1,0,0),    // Rotate from +Z to -X
	/* FACE_ZP (+Z) */ mat3(1,0,0, 0,1,0, 0,0,1),     // Identity (already facing +Z)
	/* FACE_Z  (-Z) */ mat3(-1,0,0, 0,1,0, 0,0,-1)    // Rotate 180° around Y
);

void main() {
	// Unpack data (6 bits per component)
	int x      = (packed_data >> 0)  & 0x3F;
	int y      = (packed_data >> 6)  & 0x3F;
	int z      = (packed_data >> 12) & 0x3F;
	int width  = (packed_data >> 18) & 0x3F;
	int height = (packed_data >> 24) & 0x3F;

	face = block_id & 0x07;
	block_id_out = (block_id >> 3) & 0x1FFF;

	// Base position in chunk space (0.5 units per voxel)
	vec3 vox_pos = vec3(x, y, z) * 0.5;

	vec3 face_offset = vec3(0.0);
	switch(face) {
		case FACE_YP: face_offset = vec3(0.0, 0.5, 0.5); break;
	// 	case FACE_Y:  face_offset = vec3(0.0, 0.0, 0.0); break;
		case FACE_XP: face_offset = vec3(0.5, 0.0, 0.5); break;
	//	case FACE_X:  face_offset = vec3(0.0, 0.0, 0.0); break;
		case FACE_ZP: face_offset = vec3(0.0, 0.0, 0.5); break;
		case FACE_Z:  face_offset = vec3(0.5, 0.0, 0.0); break;
	}

	vec3 rotated_aPos = ROTATIONS[face] * aPos;

	// Final position calculation
	vec3 world_pos = vox_pos +face_offset + chunk_pos + rotated_aPos;
	gl_Position = matProjection * matView * matModel * vec4(world_pos, 1.0);

	frag_pos = world_pos;
	face_normal = NORMALS[face];
}
