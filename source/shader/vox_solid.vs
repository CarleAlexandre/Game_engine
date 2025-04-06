#version 460 core

#define FACE_YP    0
#define FACE_Y     1
#define FACE_XP    2
#define FACE_X     3
#define FACE_ZP    4
#define FACE_Z     5

layout(location = 0) in vec3	aPos;
layout(location = 1) in int	packed_data;
layout(location = 2) in int	block_id;
layout(std430, binding = 3) buffer ssbo {
	int ssbo_data[];
};

uniform mat4	mvp;
uniform mat4	matView;
uniform mat4	matProjection;
uniform mat4	matModel; 

out vec3	frag_pos;
out vec3	face_normal;
flat out int	block_id_out;
flat out int	face;

const vec3 NORMALS[6] = vec3[](
	vec3( 0.0,  1.0,  0.0),
	vec3( 0.0, -1.0,  0.0),
	vec3( 1.0,  0.0,  0.0),
	vec3(-1.0,  0.0,  0.0),
	vec3( 0.0,  0.0,  1.0),
	vec3( 0.0,  0.0, -1.0)
);

const mat3 ROTATIONS[6] = mat3[](
	mat3(1,0,0, 0,0,-1, 0,1,0),
	mat3(1,0,0, 0,0,1, 0,-1,0),
	mat3(0,0,-1, 0,1,0, 1,0,0),
	mat3(0,0,1, 0,1,0, -1,0,0),
	mat3(1,0,0, 0,1,0, 0,0,1),
	mat3(-1,0,0, 0,1,0, 0,0,-1)
);

const vec3 FACE_OFFSET[6] = vec3[](
	vec3(0.0, 0.5, 0.5),
	vec3(0.0, 0.0, 0.0),
	vec3(0.5, 0.0, 0.5),
	vec3(0.0, 0.0, 0.0),
	vec3(0.0, 0.0, 0.5),
	vec3(0.5, 0.0, 0.0)
);

void main() {
	int x      = (packed_data >> 0)  & 0x3F;
	int y      = (packed_data >> 6)  & 0x3F;
	int z      = (packed_data >> 12) & 0x3F;
	int width  = (packed_data >> 18) & 0x3F;
	int height = (packed_data >> 24) & 0x3F;

	face = block_id & 0x07;
	block_id_out = (block_id >> 3) & 0x1FFF;

	vec3 vox_pos = vec3(x, y, z) * 0.5;
	vec3 vPos = vec3(aPos.x * width, aPos.y * height, aPos.z);
	vec3 rotated_aPos = ROTATIONS[face] * vPos;


	int packed_pos = ssbo_data[gl_DrawID];//need to be called via multi draw command ???
	vec3 chunk_pos = vec3((packed_pos & 0x1f), ((packed_pos >> 5) & 0x1f), ((packed_pos >> 10) & 0x1f));

	vec3 world_pos = vox_pos + FACE_OFFSET[face] + chunk_pos + rotated_aPos;
	gl_Position = matProjection * matView * matModel * vec4(world_pos, 1.0);

	frag_pos = world_pos;
	face_normal = NORMALS[face];
}
