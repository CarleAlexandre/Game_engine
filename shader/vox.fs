#version 330 core

out vec4 frag_color;

flat in int extra;
flat in int face;
in vec2 fragTexCoord;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform vec2 resolution = vec2(800, 450);

vec4 sobel_filter(void) {
	float x = 1.0/resolution.x;
	float y = 1.0/resolution.y;

	vec4 horizEdge = vec4(0.0);
	horizEdge -= texture2D(texture0, vec2(fragTexCoord.x - x, fragTexCoord.y - y))* 1.0;
	horizEdge -= texture2D(texture0, vec2(fragTexCoord.x - x, fragTexCoord.y    ))* 2.0;
	horizEdge -= texture2D(texture0, vec2(fragTexCoord.x - x, fragTexCoord.y + y))* 1.0;
	horizEdge += texture2D(texture0, vec2(fragTexCoord.x + x, fragTexCoord.y - y))* 1.0;
	horizEdge += texture2D(texture0, vec2(fragTexCoord.x + x, fragTexCoord.y    ))* 2.0;
	horizEdge += texture2D(texture0, vec2(fragTexCoord.x + x, fragTexCoord.y + y))* 1.0;

	vec4 vertEdge = vec4(0.0);
	vertEdge -= texture2D(texture0, vec2(fragTexCoord.x - x, fragTexCoord.y - y))* 1.0;
	vertEdge -= texture2D(texture0, vec2(fragTexCoord.x    , fragTexCoord.y - y))* 2.0;
	vertEdge -= texture2D(texture0, vec2(fragTexCoord.x + x, fragTexCoord.y - y))* 1.0;
	vertEdge += texture2D(texture0, vec2(fragTexCoord.x - x, fragTexCoord.y + y))* 1.0;
	vertEdge += texture2D(texture0, vec2(fragTexCoord.x    , fragTexCoord.y + y))* 2.0;
	vertEdge += texture2D(texture0, vec2(fragTexCoord.x + x, fragTexCoord.y + y))* 1.0;

	vec3 edge = sqrt((horizEdge.rgb*horizEdge.rgb) + (vertEdge.rgb*vertEdge.rgb));

	return (vec4(edge, texture2D(texture0, fragTexCoord).a));
}


void main() {
	// vec3 NORMALS[6] = {
	// 	vec3( 0.0,  1.0,  0.0),
	// 	vec3( 0.0, -1.0,  0.0),
	// 	vec3( 1.0,  0.0,  0.0),
	// 	vec3(-1.0,  1.0,  0.0),
	// 	vec3( 0.0,  0.0,  1.0),
	// 	vec3( 0.0,  0.0, -1.0)
	// };
	// vec3 normal = NORMALS[face];

	if (extra == 1) {
		vec3 color = vec3(0.0);
		if (face == 0) color = vec3(0.5, 0.0, 0.0); // Top face (red)
		else if (face == 1) color = vec3(0.0, 0.5, 0.0); // Bottom face (green)
		else if (face == 2) color = vec3(0.2, 0.2, 0.5); // Left face (blue)
		else if (face == 3) color = vec3(0.5, 0.5, 0.0); // Right face (yellow)
		else if (face == 4) color = vec3(0.5, 0.0, 0.5); // Front face (magenta)
		else if (face == 5) color = vec3(0.0, 0.5, 0.5); // Back face (cyan)
		frag_color = vec4(color, 1.0);
	}
	if (extra == 2) {
		frag_color = vec4(0.0, 0.0, 0.5, 0.5);
	}
}