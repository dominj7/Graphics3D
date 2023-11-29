#pragma once
#include <vector>
#include "glad/gl.h"
#include "Application/application.h"
#include "camera.h"
#include "camera_controller.h"
#include <glm//glm.hpp>


class SimpleShapeApplication : public xe::Application {
public:
    SimpleShapeApplication(int width, int height, std::string title, bool debug) : Application(width, height, title, debug), camera_(nullptr) {}

    void init() override;
    void frame() override;
    void framebuffer_resize_callback(int w, int h) override;
    void mouse_button_callback(int button, int action, int mods) override;
    void cursor_position_callback(double x, double y) override;

    void set_camera(Camera* camera) { camera_ = camera; }

    Camera* camera() const {
        assert(camera_);
        return camera_;
    }

    void scroll_callback(double xoffset, double yoffset) override;
    void set_controler(CameraController *controller) { controller_ = controller; }


private:
    GLuint vao_;

    GLuint u_trans_buffer_handle_;

    CameraController* controller_;
    Camera* camera_;
    glm::mat4 M_;

};
