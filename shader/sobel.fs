#version 430

in vec2 fragTexCoord;
in vec4 fragColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

out vec4 finalColor;

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

void main(void) {
	finalColor = fragColor - sobel_filter();
}
