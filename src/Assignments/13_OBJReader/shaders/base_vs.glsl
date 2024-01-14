#version 420 core

layout(location = 0) in vec4 vertexPosition;
layout(location = 1) in vec2 texCoord0; 


//out vec4 fragColor; // Varying variable to pass color to the fragment shader
out vec2 fragTexCoord0; // Varying variable to pass texture coordinates to the fragment shader

layout(std140, binding = 1) uniform Transformations {
    mat4 PVM;
};

void main() {
    gl_Position = PVM * vertexPosition;
    fragTexCoord0 = texCoord0; 
   // fragColor = vertexColor; 
}

