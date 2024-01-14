#version 420 core


in vec4 fragColor; // Input color from the vertex shader
out vec4 outColor; // Output color


layout(std140, binding=0) uniform KdMaterial {
    vec4 Kd;
    bool use_vertex_color; 
};

void main() {
    vec3 color = vec3(1,1,1);
    if(use_vertex_color){
        color *= fragColor.rgb ;
    }
    color *= Kd.rgb;
    outColor.rgb = color;
    
}