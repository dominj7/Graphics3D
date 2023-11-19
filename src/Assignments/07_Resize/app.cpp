// Created by pbialas on 25.09.2020.

#include "app.h"
#include "spdlog/spdlog.h"
#include "glad/gl.h"
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "Application/utils.h"
#include <array>


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
        static constexpr Color red{ 1.f, 0.f, 0.f };
        static constexpr Color green{ 0.f, 1.f, 0.f };
        static constexpr Color blue{ 0.f, 0.f, 1.f };
        static constexpr Color yellow{ 1.f, 1.f, 0.f };
        static constexpr Color gray{ 0.5f, 0.5f, 0.5f };
        static constexpr Color lightGray{ 0.8f, 0.8f, 0.8f };
    } // Colors
} // Utilities


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


    static constexpr Utilities::Position topPos{ 0.f, 0.f, 1.f };
    static constexpr Utilities::Position bottomLeftPos{ -0.5f, -0.5f, 0.f };
    static constexpr Utilities::Position bottomRightPos{ 0.5f, -0.5f, 0.f };
    static constexpr Utilities::Position topRightPos{ 0.5f, 0.5f, 0.f };
    static constexpr Utilities::Position topLeftPos{ -0.5f, 0.5f, 0.f };
    static const Utilities::Triangle leftTriangle{ bottomLeftPos, topPos, topLeftPos, Utilities::Colors::green };
    static const Utilities::Triangle bottomTiangle{ bottomLeftPos, bottomRightPos, topPos, Utilities::Colors::yellow };
    static const Utilities::Triangle rightTriangle{ topPos, bottomRightPos, topRightPos, Utilities::Colors::red };
    static const Utilities::Triangle topTriangle{ topLeftPos, topPos, topRightPos, Utilities::Colors::blue };
    static const Utilities::Triangle baseFirstTriangle{ bottomLeftPos, topLeftPos, topRightPos, Utilities::Colors::gray };
    static const Utilities::Triangle baseSecondTriangle{ bottomLeftPos, topRightPos, bottomRightPos, Utilities::Colors::gray };
    const auto data{ Utilities::generateTrianglesData({leftTriangle, bottomTiangle, rightTriangle, topTriangle, baseFirstTriangle, baseSecondTriangle}) };

    auto vertices{ data.first };
    auto indices{ data.second };


    GLuint vertexBufferHandle;
    OGL_CALL(glGenBuffers(1, &vertexBufferHandle));
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle));
    OGL_CALL(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW));
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

    GLuint indexBufferHandle;
    OGL_CALL(glGenBuffers(1, &indexBufferHandle));
    OGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferHandle));
    OGL_CALL(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW));
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));

    OGL_CALL(glGenVertexArrays(1, &vao_));
    OGL_CALL(glBindVertexArray(vao_));
    OGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferHandle));
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle));


    OGL_CALL(glEnableVertexAttribArray(0));
    OGL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat),
        reinterpret_cast<GLvoid*>(0)));

    OGL_CALL(glEnableVertexAttribArray(1));
    OGL_CALL(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat),
        reinterpret_cast<GLvoid*>(3 * sizeof(GLfloat))));

    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    OGL_CALL(glBindVertexArray(0));


    // --PVM--
    static constexpr size_t bufferPVMSize{ 16 * sizeof(float) };
    // Create a buffer and bind it to GL_UNIFORM_BUFFER
    OGL_CALL(glGenBuffers(1, &u_trans_buffer_handle_));
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, u_trans_buffer_handle_));
    // Allocate memory for the buffer based on the calculated size
    OGL_CALL(glBufferData(GL_UNIFORM_BUFFER, bufferPVMSize, nullptr, GL_STATIC_DRAW));

    auto [w, h] = frame_buffer_size();
    OGL_CALL(glViewport(0, 0, w, h));
    aspect_ = static_cast<float>(w) / static_cast<float>(h);
    fov_ = glm::radians(45.0f);
    near_ = 0.1f;
    far_ = 20.f;

    auto cameraPosition = glm::vec3{ 2.f, 1.f, 2.f };
    auto target = glm::vec3{ 0.f, 0.f, 0.f };
    auto upVector = glm::vec3{ 0.f, 0.f, 1.f };
    V_ =  glm::lookAt(cameraPosition, target, upVector);

    auto translation{ glm::vec3{ 0.f, 0.f, 0.f } };
    M_ = glm::mat4(1.f);
    M_ = glm::translate(M_, translation);


    OGL_CALL(glClearColor(Utilities::Colors::lightGray.r, Utilities::Colors::lightGray.g, Utilities::Colors::lightGray.b, Utilities::Colors::lightGray.a));

    OGL_CALL(glUseProgram(program));

    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
}


void SimpleShapeApplication::frame() {
    P_ = glm::perspective(fov_, aspect_, near_, far_);
    const glm::mat4 PVM{ P_ * V_ * M_ };
    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 1, u_trans_buffer_handle_));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &PVM[0]));
    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0, u_trans_buffer_handle_));

    OGL_CALL(glBindVertexArray(vao_));
    glDrawElements(GL_TRIANGLES, 18, GL_UNSIGNED_INT, nullptr);
    OGL_CALL(glBindVertexArray(0));
}

void SimpleShapeApplication::framebuffer_resize_callback(int w, int h) {
    Application::framebuffer_resize_callback(w, h);
    aspect_ = static_cast<float>(w) / static_cast<float>(h);
    OGL_CALL(glViewport(0, 0, w, h));
}

