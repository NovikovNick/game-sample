cmake_minimum_required(VERSION 3.20)
project(game)

set(CMAKE_CXX_STANDARD 17)
set(SOURCE_EXE src/main/cpp/main.cpp
        src/main/cpp/common/impl/shader.cpp
        src/main/cpp/common/shader.h)

find_package(OpenGL REQUIRED)
if (NOT OPENGL_FOUND)
    message("ERROR: OpenGL not found")
endif (NOT OPENGL_FOUND)


find_package(GLEW REQUIRED)


include_directories(
        external_dependencies/glm
)

add_executable(game ${SOURCE_EXE})

message("[debug] GLEW = " + ${GLEW_LIBRARIES})

target_link_libraries(game glfw GLEW libGLEW.so libGLU.so libGL.so)
