#version 420 core

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec4 vertexColor;

out vec4 fragColor; // Varying variable to pass color to the fragment shader

layout(std140, binding=1) uniform Transformations {
    vec2 scale;
    vec2 translation;
    mat2 rotation;
 };

void main() {
    gl_Position.xy = rotation * (scale * vertexPosition.xy) + translation;
    gl_Position.zw = vertexPosition.zw;  

    fragColor = vertexColor; // Pass the color to the fragment shader
}
