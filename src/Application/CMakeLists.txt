cmake_minimum_required(VERSION 3.15)
project(application CXX)

add_library(${PROJECT_NAME}
        application.cpp
        application.h
        utils.cpp
        utils.h
        shader_source.cpp 
        shader_source.h
        RegisteredObject.cpp
        RegisteredObject.h
        stb.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE spdlog::spdlog)