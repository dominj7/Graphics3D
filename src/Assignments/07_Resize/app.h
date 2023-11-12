//
// Created by pbialas on 05.08.2020.
//

#pragma once
#include <vector>
#include "glad/gl.h"
#include "Application/application.h"


class SimpleShapeApplication : public xe::Application {
public:
    SimpleShapeApplication(int width, int height, std::string title, bool debug) : Application(width, height, title,
                                                                                               debug) {}

    void init() override;
    void frame() override;
    void framebuffer_resize_callback(int w, int h) override;

private:
    GLuint vao_;

};