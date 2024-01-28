#include "BlinnPhongMaterial.h"

GLint xe::BlinnPhongMaterial::map_Kd_location_;
glm::vec4 xe::BlinnPhongMaterial::colorAmbient;

void xe::BlinnPhongMaterial::init(){

    create_material_uniform_buffer(4*sizeof(glm::vec4));
    create_program_in_project({{GL_VERTEX_SHADER, "BlinnPhong_vs.glsl"},
                           {GL_FRAGMENT_SHADER, "BlinnPhong_fs.glsl"}});

    map_Kd_location_ = glGetUniformLocation(program(),"map_Kd");
        if (map_Kd_location_ == -1) {
            SPDLOG_WARN("Cannot find map_Kd uniform");
            }

    xe::add_mat_function("BlinnPhongMaterial", BlinnPhongMaterial::create_from_mtl);
}

void xe::BlinnPhongMaterial::bind() const {
    glUseProgram(program());
    int use_map_Kd = 0; 
    if (Textura_ > 0)
    {
        use_map_Kd = 1;
        OGL_CALL(glActiveTexture(GL_TEXTURE0));
        OGL_CALL(glBindTexture(GL_TEXTURE_2D, Textura_));

    }
    
    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0 , material_uniform_buffer()));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &colorAmbient));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(glm::vec4), &Kd_));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4), sizeof(int), &use_vertex_colors_));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::vec4)+sizeof(int), sizeof(int), &use_map_Kd));

 }
 void xe::BlinnPhongMaterial::unbind() const {
     OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0 , 0));
     if (Textura_ > 0){
         OGL_CALL(glBindTexture(GL_TEXTURE_2D, 0 ));
     }

 }
 xe::Material *xe::BlinnPhongMaterial::create_from_mtl(const mtl_material_t &mat, std::string mtl_dir) {
     glm::vec4 color = get_color(mat.diffuse);
     colorAmbient = get_color(mat.ambient);
     SPDLOG_DEBUG("Adding ColorMaterial {}", glm::to_string(color));
     auto material = new xe::BlinnPhongMaterial(color);
    

     if (!mat.diffuse_texname.empty()) {
         auto texture = xe::create_texture(mtl_dir + "/" + mat.diffuse_texname, true);
         SPDLOG_DEBUG("Adding Texture {} {:1d}", mat.diffuse_texname, texture);
         if (texture > 0) {
             material->set_texture(texture);
         }
     }

     return material;
}