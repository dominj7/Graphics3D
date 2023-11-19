//
// Created by pbialas on 05.08.2020.
//

#pragma once
#include <vector>
#include "glad/gl.h"
#include "Application/application.h"
#include <glm//glm.hpp>


class SimpleShapeApplication : public xe::Application {
public:
    SimpleShapeApplication(int width, int height, std::string title, bool debug) : Application(width, height, title,
                                                                                               debug) {}

    void init() override;
    void frame() override;
    void framebuffer_resize_callback(int w, int h) override;

private:
    GLuint vao_;

    GLuint u_trans_buffer_handle_;

    float fov_;     // field of view
    float aspect_;  // aspect ratio
    float near_;    // near plane
    float far_;     // far plane

    glm::mat4 P_;
    glm::mat4 V_;
    glm::mat4 M_;

};