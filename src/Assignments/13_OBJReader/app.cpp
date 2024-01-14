// Created by pbialas on 25.09.2020.

#include "app.h"
#include "spdlog/spdlog.h"
#include "glad/gl.h"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Application/utils.h"
#include "Engine/KdMaterial.h"
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
    

    std::vector<GLfloat> vertices = {
            -0.5f, 0.5f, 0.0f, 0.1910f, 0.5f,
            0.5f, 0.5f, 0.0f, 0.5f, 0.8090f,
            0.5f, -0.5f, 0.0f, 0.8090f, 0.5f,
            -0.5f, -0.5f, 0.0f, 0.5f, 0.1910f,
            0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
            };

    std::vector<GLubyte> indices = {
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        3, 2, 5,
        1, 0, 6,
        2, 1, 7
    };


    stbi_set_flip_vertically_on_load(true);
    GLint width, height, channels;
    auto texture_file = std::string(ROOT_DIR) + "/Models/multicolor.png";
    auto img = stbi_load(texture_file.c_str(), &width, &height, &channels, 0);
    if (!img) {
    std::cerr<<"Could not read image from file `"<<texture_file<<"'\n";
    } else {
    std::cout<<"Loaded a "<<width<<"x"<<height<<" texture with "<<channels<<" channels\n";
    }

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);

        // Set texture parameters (filtering methods)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        // Load the image data into the texture
    glTexImage2D(GL_TEXTURE_2D, 0, GL_SRGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
    glBindTexture(GL_TEXTURE_2D, 0);

        // Free the image data
    stbi_image_free(img);

    // --PVM--
    static constexpr size_t bufferPVMSize{ 16 * sizeof(float) };
    // Create a buffer and bind it to GL_UNIFORM_BUFFER
    OGL_CALL(glGenBuffers(1, &u_trans_buffer_handle_));
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, u_trans_buffer_handle_));
    // Allocate memory for the buffer based on the calculated size
    OGL_CALL(glBufferData(GL_UNIFORM_BUFFER, bufferPVMSize, nullptr, GL_STATIC_DRAW));
    
    set_camera(new xe::Camera);
    set_controler(new xe::CameraController(camera()));
    
    auto cameraPosition = glm::vec3{ 2.f, 1.f, 2.f };
    auto target = glm::vec3{ 0.f, 0.f, 0.f };
    auto upVector = glm::vec3{ 0.f, 0.f, 1.f };
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


    auto pyramid = xe::load_mesh_from_obj(std::string(ROOT_DIR) + "/Models/blue_marble.obj", std::string(ROOT_DIR) + "/Models");
    add_mesh(pyramid);

    

    OGL_CALL(glClearColor(0.81, 0.81, 0.81, 1.0 ));

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}


void SimpleShapeApplication::frame() {
    glm::mat4 PVM{ camera_->projection() * camera_->view() * M_ };
    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_trans_buffer_handle_));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]));

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

