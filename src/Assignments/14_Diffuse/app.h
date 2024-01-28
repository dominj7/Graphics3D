#pragma once
#include <vector>
#include "glad/gl.h"
#include "Application/application.h"
#include "Engine/camera.h"
#include "Engine/camera_controller.h"
#include "Engine/Mesh.h"
#include <glm//glm.hpp>
#include "Engine/light.h"



class SimpleShapeApplication : public xe::Application {
public:
    SimpleShapeApplication(int width, int height, std::string title, bool debug) : Application(width, height, title, debug), camera_(nullptr) {}

    void init() override;
    void frame() override;
    void framebuffer_resize_callback(int w, int h) override;
    void mouse_button_callback(int button, int action, int mods) override;
    void cursor_position_callback(double x, double y) override;


    void set_camera(xe::Camera* camera) { camera_ = camera; }

    xe::Camera* camera() const {
        assert(camera_);
        return camera_;
    }

    void scroll_callback(double xoffset, double yoffset) override;
    xe::CameraController *controller_;
    void set_controler(xe::CameraController *controller) { controller_ = controller; }
    
    
    std::vector<xe::Mesh*> meshes_; 

    void add_mesh(xe::Mesh *mesh) {
        meshes_.push_back(mesh);
    }
    std::vector<xe::PointLight> lights_;
    void add_light(xe::PointLight light) { lights_.push_back(light); }


private:
    GLuint vao_;
    GLuint u_light_buffer_handle_;
    GLuint u_trans_buffer_handle_;

    xe::Camera* camera_;
    glm::mat4 M_;

};
