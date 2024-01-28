#version 420

layout(location = 0) in vec4 a_vertex_position;
layout(location = 5) in vec4 a_vertex_color;
layout(location = 3) in vec2 a_vertex_texcoord_0;
layout(location = 1) in vec3 a_vertex_normal;

out vec4 vertex_color;
out vec2 vertex_texcoord_0;
out vec3 vertex_normal_vs;
out vec3 vertex_position_vs;

layout(std140, binding = 1) uniform Transformations {
    mat4 PVM;
    mat4 VM; 
    mat3 VM_normal; // VM_Normal matrix
};

void main() {
    vertex_color = a_vertex_color;
    vertex_texcoord_0 = a_vertex_texcoord_0;

    vec4 a_vertex_position_vs = VM * a_vertex_position;
    vertex_position_vs = a_vertex_position_vs.xyz/a_vertex_position_vs.w;

    vertex_normal_vs = normalize(VM_normal * a_vertex_normal);


    // Calculate the gl_Position in clip space
    gl_Position = PVM * a_vertex_position;
}
