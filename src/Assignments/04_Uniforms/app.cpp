// Created by pbialas on 25.09.2020.

#include "app.h"
#include "spdlog/spdlog.h"
#include "glad/gl.h"
#include "Application/utils.h"


void SimpleShapeApplication::init() {
    auto program = xe::utils::create_program(
    {
        {GL_VERTEX_SHADER,   std::string(PROJECT_DIR) + "/shaders/base_vs.glsl"},
        {GL_FRAGMENT_SHADER, std::string(PROJECT_DIR) + "/shaders/base_fs.glsl"}
    });

    if (!program) {
        SPDLOG_CRITICAL("Invalid program");
        exit(-1);
    }


    std::vector<GLfloat> vertices = {
        -0.5f, 0.0f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,

        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f
     };

    std::vector<GLuint> indices = {
        0, 1, 2,
        3, 4, 5,
        6 ,4, 3
    };


    GLuint v_buffer_handle;
    OGL_CALL(glGenBuffers(1, &v_buffer_handle));
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle));
    OGL_CALL(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW));
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

    GLuint elementbuffer;
    OGL_CALL(glGenBuffers(1, &elementbuffer));
    OGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer));
    OGL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW));
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

    OGL_CALL(glGenVertexArrays(1, &vao_));
    OGL_CALL(glBindVertexArray(vao_));
    OGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elementbuffer));
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, v_buffer_handle));


    OGL_CALL(glEnableVertexAttribArray(0));
    OGL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat),
                                   reinterpret_cast<GLvoid *>(0)));

    OGL_CALL(glEnableVertexAttribArray(1));
    OGL_CALL(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat),
                                    reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat))));

    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    OGL_CALL(glBindVertexArray(0));



    OGL_CALL(glClearColor(0.81f, 0.81f, 0.8f, 1.0f));

    auto [w, h] = frame_buffer_size();
    OGL_CALL(glViewport(0, 0, w, h));

    OGL_CALL(glUseProgram(program));
}


void SimpleShapeApplication::frame() {
    OGL_CALL(glBindVertexArray(vao_));
    glDrawElements( GL_TRIANGLES, 9, GL_UNSIGNED_INT, nullptr );
    OGL_CALL(glBindVertexArray(0));
}

