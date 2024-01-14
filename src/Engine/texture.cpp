#include "texture.h"

GLuint xe::create_texture(const std::string& name, bool is_sRGB) {
    stbi_set_flip_vertically_on_load(true);
    GLint width, height, channels;
    auto texture_file = name;
    auto img = stbi_load(texture_file.c_str(), &width, &height, &channels, 0);
    if (!img) {
        std::cerr<<"Could not read image from file `"<<texture_file<<"'\n";
    } else {
        std::cout<<"Loaded a "<<width<<"x"<<height<<" texture with "<<channels<<" channels\n";
    }

    GLuint gl_textures;
    
    OGL_CALL(glGenTextures(1, &gl_textures));

    glBindTexture(GL_TEXTURE_2D, gl_textures);

    OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));

    OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));

    OGL_CALL(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    
    if (is_sRGB){
        OGL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img));
    }
    else{
        OGL_CALL(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img));
    }
    
    glBindTexture(GL_TEXTURE_2D, 0); 

    stbi_image_free(img);

    return gl_textures;
}