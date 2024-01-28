#version 420 core

in vec4 fragColor; // Input color from the vertex shader
in vec2 fragTexCoord0; 
out vec4 outColor; // Output color
uniform sampler2D map_Kd; 
vec3 srgb_gamma_correction(vec3 color) {
   color = clamp(color, 0.0, 1.0);
   color = mix(color * 12.92, (1.055 * pow(color, vec3(1.0 / 2.4))) - 0.055, step(0.0031308, color));
   return color;
}


layout(std140, binding=0) uniform KdMaterial {
    vec4 Kd;
    bool use_vertex_color; 
    bool use_map_Kd; 
    //vec4 texture_color = texture(map_Kd, vertex_texcoord_0); 
};

void main() {
    fragColor = texture(map_Kd, fragTexCoord0)
    //vFragColor.a = color.a;
    //vFragColor.rgb = srgb_gamma_correction(color.rgb);
}


