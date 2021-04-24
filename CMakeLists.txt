cmake_minimum_required(VERSION 3.0.0)
project(opengl VERSION 0.1.0)

set(CMAKE_BUILD_TYPE Debug)

find_package(OpenGL REQUIRED)

add_executable(${PROJECT_NAME} main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw GLEW ${OPENGL_LIBRARIES})
