// Created by pbialas on 25.09.2020.

#include "app.h"
#include "spdlog/spdlog.h"
#include "glad/gl.h"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Application/utils.h"
#include "Engine/KdMaterial.h"
#include "Engine/BlinnPhongMaterial.h"
#include <array>
#include "stb/stb_image.h"


using GLuintVec = std::vector<GLuint>;
using GLfloatVec = std::vector<GLfloat>;

namespace Utilities {
    struct Position {
        GLfloat x{}, y{}, z{};

        bool operator==(const Position& other) const {
            return x == other.x && y == other.y && z == other.z;
        }
    };

    struct Color {
        GLfloat r{}, g{}, b{}, a{ 1.0f };

        bool operator==(const Color& other) const {
            return r == other.r && g == other.g && b == other.b && a == other.a;
        }
    };

    struct Texture
    {
        GLfloat x{}, y{};

        bool operator==(const Texture& other) const {
            return x == other.x && y == other.y;
        }
    };
    

    struct Vertex {
        Position position{};
        Color color{};

        GLfloatVec getData() const {
            GLfloatVec verticesData{};
            verticesData.push_back(position.x);
            verticesData.push_back(position.y);
            verticesData.push_back(position.z);
            verticesData.push_back(color.r);
            verticesData.push_back(color.g);
            verticesData.push_back(color.b);
            verticesData.push_back(color.a);

            return verticesData;
        }

        bool operator==(const Vertex& other) const {
            return position == other.position && color == other.color;
            }
    };

    class Triangle {
    public:
        Triangle(Vertex first, Vertex second, Vertex third) {
            vertices = { first, second, third };
            
        }

       

        Triangle(Position first, Position second, Position third, Color color = { 0.f, 0.f, 0.f }) {
            vertices = { Vertex{ first, color }, Vertex{ second, color }, Vertex{ third, color } };
        }

        const std::array<Vertex, 3>& getVertices() const {
            return vertices;
        }

    private:
        std::array<Vertex, 3> vertices;
    };

    std::pair<GLfloatVec, GLuintVec> generateTrianglesData(const std::vector<Triangle>& triangles) {
        std::vector<Vertex> vertices{};
        GLuintVec indices{};
        GLuint indexCounter{};

        for (const auto& triangle : triangles) {
            for (const auto& triangleVertex : triangle.getVertices()) {
                if (auto it = std::find_if(std::begin(vertices), std::end(vertices), [triangleVertex](Vertex vertex) {
                    return triangleVertex == vertex;
                    }); it == std::end(vertices)) {
                    vertices.push_back(triangleVertex);
                    indices.push_back(indexCounter++);
                }
                else {
                    indices.push_back(std::distance(std::begin(vertices), it));
                }
            }
        }

        GLfloatVec verticesData{};
        for (const auto& vertex : vertices) {
            const auto vertexData{ vertex.getData() };
            verticesData.insert(std::end(verticesData), std::begin(vertexData), std::end(vertexData));
        }

        return std::pair<GLfloatVec, GLuintVec>{verticesData, indices};
    }

    namespace Colors {
        static constexpr Color red_top{ 1.f, 0.f, 0.f };
        static constexpr Color green_top{ 1.f, 1.f, 0.f };
        static constexpr Color blue_top{ 0.f, 0.f, 0.f };
        static constexpr Color orange_top{ 0.0f, 1.0f, 0.0f };
        static constexpr Color red_green_bottom{ 0.1910, 0.5000, 0.0f };
        static constexpr Color red_blue_bottom{ 0.5000, 0.8090, 0.0f };
        static constexpr Color orange_bluebottom{ 0.8090 , 0.5000, 0.0f };
        static constexpr Color orange_green_bottom{ 0.5000, 0.1910, 0.0f };
    } // Colors

} // Utilities


void SimpleShapeApplication::init() {
    
    xe::KdMaterial::init();
    xe::BlinnPhongMaterial::init();

    // --PVM--
    static constexpr size_t bufferPVMSize{ 64 * sizeof(float) };

    OGL_CALL(glGenBuffers(1, &u_trans_buffer_handle_));
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, u_trans_buffer_handle_));
    OGL_CALL(glBufferData(GL_UNIFORM_BUFFER, bufferPVMSize, nullptr, GL_STATIC_DRAW));

    static constexpr size_t bufferLightSize{ 2*sizeof(glm::vec3)+xe::MAX_POINT_LIGHTS*sizeof(xe::PointLight) }; // Assuming a single vec3 for ambient light

    OGL_CALL(glGenBuffers(1, &u_light_buffer_handle_));
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, u_light_buffer_handle_));
    OGL_CALL(glBufferData(GL_UNIFORM_BUFFER, bufferLightSize, nullptr, GL_STATIC_DRAW));

    xe::PointLight light = xe::PointLight(glm::vec3(0,0,1), glm::vec3(1,1,1), 1.0, 0.1); 
    add_light(light);
    

    set_camera(new xe::Camera);
    set_controler(new xe::CameraController(camera()));
    
    auto cameraPosition = glm::vec3{ 0.f, 0.f, 3.f };
    auto target = glm::vec3{ 0.f, 0.f, 0.f };
    auto upVector = glm::vec3{ 0.f, 1.f, 0.f };
    camera()->look_at(cameraPosition, target, upVector);

    auto [w, h] = frame_buffer_size();
    OGL_CALL(glViewport(0, 0, w, h));
    float aspect = static_cast<float>(w) / static_cast<float>(h);
    float fov = glm::radians(45.0f);
    float near{ 0.1f };
    float far{ 20.f };
    camera()->perspective(fov, aspect, near, far);

   
    auto translation{ glm::vec3{ 0.f, 0.f, 0.f } };
    M_ = glm::mat4(1.f);
    M_ = glm::translate(M_, translation);

    auto pyramid = xe::load_mesh_from_obj(std::string(ROOT_DIR) + "/Models/square.obj", std::string(ROOT_DIR) + "/Models");
    add_mesh(pyramid);
    
    OGL_CALL(glClearColor(0.81, 0.81, 0.81, 1.0 ));

    

    glEnable(GL_DEPTH_TEST);
}


void SimpleShapeApplication::frame() {
      // Set VM matrix to identity
    glm::mat4 VM = camera_->view() * M_ ;

    glm::mat4 PVM{ camera_->projection() * camera_->view() * M_ };

    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_trans_buffer_handle_));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &VM[0]));

    auto R = glm::mat3(VM);
    auto VM_Normal = glm::mat3(glm::cross(R[1], R[2]), glm::cross(R[2], R[0]), glm::cross(R[0], R[1]));

    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), sizeof(glm::vec3), &VM_Normal[0]));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + sizeof(glm::vec4), sizeof(glm::vec3), &VM_Normal[1]));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4) + 2*sizeof(glm::vec4), sizeof(glm::vec3), &VM_Normal[2]));


    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 2, u_light_buffer_handle_));
    glm::vec3 initialAmbientLight(0.25f, 0.25f, 0.25f); // Example ambient light data
    int lights_number = lights_.size() ;
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::vec3), &initialAmbientLight));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec3), sizeof(int), &lights_number));

    int counter = 0;
    for(auto light:lights_){
        xe::PointLight l_transformed = xe::transform(light,camera()->view());
        OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::vec3)+sizeof(int)+counter*sizeof(xe::PointLight), sizeof(xe::PointLight), &l_transformed));

    };

    for (auto m: meshes_)
        m->draw();
    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 1, 0));
}


void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    camera_->set_aspect(static_cast<float>(w) / static_cast<float>(h));
    OGL_CALL(glViewport(0, 0, w, h));
}

void SimpleShapeApplication::scroll_callback(double xoffset, double yoffset) {
    Application::scroll_callback(xoffset, yoffset);
    static constexpr float sensitive{ 20.f };
    camera()->zoom( yoffset / sensitive);
}

void SimpleShapeApplication:: mouse_button_callback(int button, int action, int mods) {
    Application::mouse_button_callback(button, action, mods);

    if (controller_) {
        double x, y;
        glfwGetCursorPos(window_, &x, &y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
            controller_->LMB_pressed(x, y);

        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
            controller_->LMB_released(x, y);
        }
    } 

void SimpleShapeApplication::cursor_position_callback(double x, double y) {
        Application::cursor_position_callback(x, y);
        if (controller_) {
            controller_->mouse_moved(x, y);
        }
    }

