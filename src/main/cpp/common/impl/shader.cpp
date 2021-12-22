#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>

#include <stdlib.h>
#include <string.h>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>


#include "../shader.h"

GLuint GLUtil::LoadShaders(const char *vertex_file_path, const char *fragment_file_path) {

    // Создаем шейдеры
    GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
    GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

    // Загружаем код Вершинного Шейдера из файла
    std::string VertexShaderCode;
    std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
    if (VertexShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << VertexShaderStream.rdbuf();
        VertexShaderCode = sstr.str();
        VertexShaderStream.close();
    }

    // Загружаем код Фрагментного шейдера из файла
    std::string FragmentShaderCode;
    std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
    if (FragmentShaderStream.is_open()) {
        std::stringstream sstr;
        sstr << FragmentShaderStream.rdbuf();
        FragmentShaderCode = sstr.str();
        FragmentShaderStream.close();
    }

    GLint Result = GL_FALSE;
    int InfoLogLength;

    // Компилируем Вершинный шейдер
    printf("Compiling vertex shader: %sn \n", vertex_file_path);
    char const *VertexSourcePointer = VertexShaderCode.c_str();
    glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
    glCompileShader(VertexShaderID);

    // Выполняем проверку Вершинного шейдера
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
        fprintf(stdout, "%sn", &VertexShaderErrorMessage[0]);
    }

    // Компилируем Фрагментный шейдер
    printf("Compiling fragment shader: %sn \n", fragment_file_path);
    char const *FragmentSourcePointer = FragmentShaderCode.c_str();
    glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
    glCompileShader(FragmentShaderID);

    // Проверяем Фрагментный шейдер
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
    glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
        glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
        fprintf(stdout, "%s\n", &FragmentShaderErrorMessage[0]);
    }

    // Создаем шейдерную программу и привязываем шейдеры к ней
    fprintf(stdout, "Create shader program and bind shaders \n");
    GLuint ProgramID = glCreateProgram();
    glAttachShader(ProgramID, VertexShaderID);
    glAttachShader(ProgramID, FragmentShaderID);
    glLinkProgram(ProgramID);

    // Проверяем шейдерную программу
    glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
    glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
    if (InfoLogLength > 0) {
        std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
        glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
        fprintf(stdout, "%s\n", &ProgramErrorMessage[0]);
    }

    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    return ProgramID;
}

glm::mat4 GLUtil::getView(GLFWwindow *window,
                          Camera &camera,
                          const Input &input,
                          const float deltaTime) {

    // tmp
    int WIDTH = 1024;
    int HEIGHT = 768;

    double xpos, ypos;
    glfwGetCursorPos(window, &xpos, &ypos);
    glfwSetCursorPos(window, WIDTH/2, HEIGHT/2);

    camera.horizontalAngle += camera.mouseSpeed * float(WIDTH/2 - xpos );
    camera.verticalAngle   += camera.mouseSpeed * float(HEIGHT/2 - ypos );

    // Direction : Spherical coordinates to Cartesian coordinates conversion
    glm::vec3 direction(
            cos(camera.verticalAngle) * sin(camera.horizontalAngle),
            sin(camera.verticalAngle),
            cos(camera.verticalAngle) * cos(camera.horizontalAngle)
    );

    // Right vector
    glm::vec3 right = glm::vec3(
            sin(camera.horizontalAngle - 3.14f/2.0f),
            0,
            cos(camera.horizontalAngle - 3.14f/2.0f)
    );

    // Up vector
    glm::vec3 up = glm::cross( right, direction );


    /*std::cout   << "------------------Camera----------------" << std::endl
                << "float horizontalAngle = " << camera.horizontalAngle  << ";\n"
                << "float verticalAngle = " << camera.verticalAngle  << ";\n"
                << "glm::vec3 position = glm::vec3(" << camera.position[0] << ", " << camera.position[1] << ", " << camera.position[2]  << ");\n";*/


    if (input.isWPressed) camera.position += direction * deltaTime * camera.mouseSpeed * 100.0f;
    if (input.isSPressed) camera.position -= direction * deltaTime * camera.mouseSpeed * 100.0f;
    if (input.isAPressed) camera.position -= right * deltaTime * camera.mouseSpeed* 100.0f;
    if (input.isDPressed) camera.position += right * deltaTime * camera.mouseSpeed* 100.0f;


    // Или, для ортокамеры
    glm::mat4 View       = glm::lookAt(
            camera.position,           // Camera is here
            camera.position + direction, // and looks here : at the same position, plus "direction"
            up                  // Head is up (set to 0,-1,0 to look upside-down)
    );
    return View;
}

Input GLUtil::getInput(GLFWwindow *window) {
    return Input(
            glfwGetKey(window, GLFW_KEY_W ) == GLFW_PRESS,
            glfwGetKey(window, GLFW_KEY_A ) == GLFW_PRESS,
            glfwGetKey(window, GLFW_KEY_S ) == GLFW_PRESS,
            glfwGetKey(window, GLFW_KEY_D ) == GLFW_PRESS
    );
}

void GLUtil::nextFrame(FrameData &frameData) {

    frameData.frame = frameData.frame + 1;
    frameData.t0 = frameData.t1;
    frameData.t1 = glfwGetTime();
}
