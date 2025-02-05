// for now this is a dummy file

in int data;

uniform vec3 world_pos;

void main() {
	vec3 NORMALS[6] = {
		vec3( 0.0,  1.0,  0.0),
		vec3( 0.0, -1.0,  0.0),
		vec3( 1.0,  0.0,  0.0),
		vec3(-1.0,  1.0,  0.0),
		vec3( 0.0,  0.0,  1.0),
		vec3( 0.0,  0.0, -1.0)
	};

	int pos_x =	data		&63;
	int pos_y =	(data >> 6)	&63;
	int pos_z =	(data >> 12)	&63;
	int face =	(data >> 18)	&7;
	int text_id =	(data >> 21)	&63;

	vec3 normal = NORMALS[face];

	vec3 vertex_pos = vec3(pos_x, pos_y, pos_z);

	vec3 finalPos = vertex_pos + world_pos;
}