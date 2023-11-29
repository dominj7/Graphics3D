#version 420 core


layout(location = 0) in vec4 vertexPosition;
layout(location = 5) in vec4 vertexColor;

out vec4 fragColor; // Varying variable to pass color to the fragment shader

layout(std140, binding = 1) uniform Transformations {
    mat4 PVM;
};

void main() {
    gl_Position =  PVM * vertexPosition;
    fragColor = vertexColor; // Pass the color to the fragment shader
}
