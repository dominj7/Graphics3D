#version 420

layout(location = 0) out vec4 vFragColor;

in vec4 vertex_color;
in vec2 vertex_texcoord_0;
in vec3 vertex_normal_vs;
in vec3 vertex_position_vs; 

struct PointLight {
  vec3 position;
  float radius;
  vec3 color;
  float intensity;
 };

const int MAX_POINT_LIGHTS = 16;

layout (std140, binding = 2) uniform Lights {
  vec3 ambient;
  int n_lights;
  PointLight lights[MAX_POINT_LIGHTS];
};

layout(std140, binding = 0) uniform Material {
    vec4 Ka;
    vec4 Kd;
    bool use_vertex_colors;
    bool use_map_Kd;
};

uniform sampler2D map_Kd;

vec3 srgb_gamma_correction(vec3 color) {
    color = clamp(color, 0.0, 1.0);
    color = mix(color * 12.92, (1.055 * pow(color, vec3(1.0 / 2.4))) - 0.055, step(0.0031308, color));
    return color;
}

void main() {

    vec3 normal = normalize(vertex_normal_vs);
    vec3 position = abs(vertex_position_vs); 
    vec4 color = vec4(1.0, 1.0, 1.0, 1.0);
    color.rgb *= Ka.rgb * ambient;
    vec4 colorKd = Kd;

  

    if (use_vertex_colors)
        colorKd *= vertex_color;

    if (use_map_Kd) {
        colorKd *= texture(map_Kd, vertex_texcoord_0);
        colorKd.rgb = srgb_gamma_correction(color.rgb);
    }
    if(!gl_FrontFacing){ 
        normal = -normal;
    }
    for(int i = 0; i<n_lights; i++ ){
       
        vec3 l_vec = normalize(lights[i].position - vertex_position_vs);
        float distance = length(lights[i].position - vertex_position_vs);
        vec3 diffuse = colorKd.rgb *  max(0.0, dot(normal, l_vec));
        float r = max(lights[i].radius, distance);
        float attenuation = 1.0 / (r * r);

        color.rgb += 1.0 / 3.14  * lights[i].color * lights[i].intensity * diffuse * attenuation;
    }

    vFragColor = color;
} 

