#version 420 core

in vec4 fragColor; // Input color from the vertex shader
out vec4 outColor; // Output color

layout(std140, binding = 0) uniform Mixer {
    float strength;
    vec3  mix_color; 
};


void main() {
    outColor.a = fragColor.a;
    outColor.rgb = strength * mix_color + fragColor.rgb;
}