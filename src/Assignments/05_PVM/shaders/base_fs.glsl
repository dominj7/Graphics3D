//#version 330 core
#version 420


in vec4 fragColor; // Input color from the vertex shader
out vec4 outColor; // Output color



layout(std140, binding = 0) uniform Mixer {
 float strength;
 vec3  mix_color; 
};


layout(location=0) out vec4 vFragColor;

void main() {
  // Separate the alpha value
    float alpha = fragColor.a;
    
    // Mix the RGB values separately
    vec3 originalRGB = fragColor.rgb;

    //outColor = fragColor; // Set the fragment color
    //vFragColor.a = fragColor.a;
    //vFragColor.rgb = strength*mix_color*fragColor.rgb;

    vec3 blendedColor ;
    blendedColor = mix(originalRGB, mix_color, strength);
    outColor = vec4(blendedColor, 1.0);
}