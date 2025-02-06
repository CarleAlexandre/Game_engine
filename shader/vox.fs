#version 330 core
in int extra; // Extra data from the vertex shader

out vec4 frag_color;

void main() {
    // Use v_extra for texture coordinates, lighting, etc.
    frag_color = vec4(float(extra) / 16384.0, 0.0, 0.0, 1.0); // Example usage
}