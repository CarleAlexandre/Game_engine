#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec4 gZ;

in vec3 fragPosition;
in vec2 fragTexCoord;
in vec3 fragNormal;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

float near = 0.1; 
float far  = 100.0; 
  
float LinearizeDepth(float depth) {
	float z = depth * 2.0 - 1.0;
	return (2.0 * near * far) / (far + near - z * (far - near));
}

void main() {
	float depth = LinearizeDepth(gl_FragCoord.z) / far;
	gZ = vec4(vec3(depth), 1.0);

	gPosition = fragPosition;

	gNormal = normalize(fragNormal);

	gAlbedoSpec.rgb = texture(diffuseTexture, fragTexCoord).rgb;
	gAlbedoSpec.a = texture(specularTexture, fragTexCoord).r;
}