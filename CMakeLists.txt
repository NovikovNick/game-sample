cmake_minimum_required(VERSION 3.20)
project(game)

add_subdirectory(external_dependencies/glfw)
add_subdirectory(external_dependencies/soil)

set(CMAKE_CXX_STANDARD 17)

set(SOURCE_EXE
        src/main/cpp/main.cpp
        src/main/cpp/common
        src/main/cpp/model
        src/main/cpp/graphic)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)


include_directories(
        external_dependencies/glm,
        external_dependencies/glfw/include
        external_dependencies/soil/src
        src/main/cpp
)

add_executable(game ${SOURCE_EXE})
add_executable(sample src/main/cpp/sample.cpp)

target_link_libraries(game glfw GLEW SOIL libGLEW.so libGLU.so libGL.so)
