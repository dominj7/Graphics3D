#version 420 core

layout(location = 1) in vec4 inColor; // Color attribute (R, G, B, A)
layout(location = 0) in vec4 a_vertex_position;  // Updated to vec3 (or appropriate type)

layout(std140, binding=1) uniform Transformations {
    mat4 PVM;
};

out vec4 fragColor; // Varying variable to pass color to the fragment shader

void main() {
   // gl_Position = a_vertex_position;
   gl_Position =  PVM * a_vertex_position;
    fragColor = inColor; // Pass the color to the fragment shader
}

