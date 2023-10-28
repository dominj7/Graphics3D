#version 330 core

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec4 inColor; // Color attribute (R, G, B, A)

out vec4 fragColor; // Varying variable to pass color to the fragment shader

void main() {
    gl_Position = vec4(inPosition, 1.0);
    fragColor = inColor; // Pass the color to the fragment shader
}
