#include "KdMaterial.h"

void xe::KdMaterial::init(){

    create_material_uniform_buffer(2*sizeof(glm::vec4));
    create_program_in_project({{GL_VERTEX_SHADER, "Kd_vs.glsl"},
                           {GL_FRAGMENT_SHADER, "Kd_fs.glsl"}});
}

void xe::KdMaterial::bind() const {
    glUseProgram(program());
    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0 , material_uniform_buffer()));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec4), &Kd_));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec4), sizeof(int), &use_vertex_colors_));


 }
 void xe::KdMaterial::unbind() const {
     OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0 , 0));

 }