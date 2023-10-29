// Created by pbialas on 25.09.2020.

#include "app.h"
#include "spdlog/spdlog.h"
#include "glad/gl.h"
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
                    indices.push_back(indexCounter);
                    ++indexCounter;
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
        static constexpr Color red{ 1.0f, 0.0f, 0.0f };
        static constexpr Color green{ 0.0f, 1.0f, 0.0f };
        static constexpr Color gray{ 0.5f, 0.5f, 0.5f };
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

    static constexpr Utilities::Position A{ -0.5f, 0.0f, 0.0f };
    static constexpr Utilities::Position B{ 0.0f, 0.5f, 0.0f };
    static constexpr Utilities::Position C{ 0.5f, 0.0f, 0.0f };
    static constexpr Utilities::Position D{ 0.5f, -0.5f, 0.0f };
    static constexpr Utilities::Position E{ -0.5f, -0.5f, 0.0f };
    Utilities::Triangle firstTriangle{ {A, Utilities::Colors::red}, {B, Utilities::Colors::red}, {C, Utilities::Colors::red} };
    Utilities::Triangle secondTriangle{ {A, Utilities::Colors::green}, {C, Utilities::Colors::green}, {E, Utilities::Colors::green} };
    Utilities::Triangle thirdTriangle{ {C, Utilities::Colors::green}, {D, Utilities::Colors::green}, {E, Utilities::Colors::green} };
    auto data{ Utilities::generateTrianglesData({firstTriangle, secondTriangle, thirdTriangle}) };

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


    // Uniforms
    GLuint uniformBufferHandle;
    OGL_CALL(glGenBuffers(1, &uniformBufferHandle));
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, uniformBufferHandle));
    static constexpr float strength{ 1.0 };
    static constexpr float mix_color[3] = { -0.5, -0.5, 0.5 };
    OGL_CALL(glBufferData(GL_UNIFORM_BUFFER, 8 * sizeof(GLfloat), nullptr, GL_STATIC_DRAW));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(GLfloat), &strength));
    OGL_CALL(glBufferSubData(GL_UNIFORM_BUFFER, 4 * sizeof(GLfloat), 3 * sizeof(float), mix_color));
    OGL_CALL(glBindBuffer(GL_UNIFORM_BUFFER, 0));
    OGL_CALL(glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniformBufferHandle));

    OGL_CALL(glGenVertexArrays(1, &vao_));
    OGL_CALL(glBindVertexArray(vao_));
    OGL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferHandle));
    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vertexBufferHandle));


    OGL_CALL(glEnableVertexAttribArray(0));
    OGL_CALL(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat),
        reinterpret_cast<GLvoid *>(0)));

    OGL_CALL(glEnableVertexAttribArray(1));
    OGL_CALL(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat),
        reinterpret_cast<GLvoid *>(3 * sizeof(GLfloat))));

    OGL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    OGL_CALL(glBindVertexArray(0));


    OGL_CALL(glClearColor(Utilities::Colors::gray.r, Utilities::Colors::gray.g, Utilities::Colors::gray.b, Utilities::Colors::gray.a));

    auto [w, h] = frame_buffer_size();
    OGL_CALL(glViewport(0, 0, w, h));

    OGL_CALL(glUseProgram(program));
}


void SimpleShapeApplication::frame() {
    OGL_CALL(glBindVertexArray(vao_));
    glDrawElements( GL_TRIANGLES, 9, GL_UNSIGNED_INT, nullptr );
    OGL_CALL(glBindVertexArray(0));
}

