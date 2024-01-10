#version 420 core

in vec4 fragColor; // Input color from the vertex shader
out vec4 outColor; // Output color


void main() {
    outColor = fragColor;
}