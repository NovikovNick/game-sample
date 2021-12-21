// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// gl math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// Include GLFW
#include <GLFW/glfw3.h>
#include <iostream>
#include <cstring>
#include "common/shader.h"

int WIDTH = 1024;
int HEIGHT = 768;

GLFWwindow *window;

int main(void) {

    if (!glfwInit()) {
        fprintf(stderr, "Failed to initialize GLFW\n");
        getchar();
        return -1;
    }

    glfwWindowHint(GLFW_SAMPLES, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Open a window and create its OpenGL context
    window = glfwCreateWindow(WIDTH, HEIGHT, "Captain Triangle conquers the universe", NULL, NULL);
    if (window == NULL) {
        fprintf(stderr,
                "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
        getchar();
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    glewExperimental = true; // Needed for core profile
    // Initialize GLEW
    if (glewInit() != GLEW_OK) {
        fprintf(stderr, "Failed to initialize GLEW\n");
        getchar();
        glfwTerminate();
        return -1;
    }

    // Ensure we can capture the escape key being pressed below
    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

    // Dark blue background
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
    glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);


    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);

    // Create and compile our GLSL program from the shaders
    GLuint programID = GLUtil::LoadShaders("../src/main/glsl/SimpleVertexShader.vertexshader",
                                           "../src/main/glsl/SimpleFragmentShader.fragmentshader");
    /*GLuint programID = GLUtil::LoadShaders("../src/main/glsl/TransformVertexShader.vertexshader",
                                            "../src/main/glsl/TextureFragmentShader.fragmentshader");*/

    // mesh
    Cube cube = Cube();
    CubeTexture cubeTexture = CubeTexture();

    GLuint vertexbuffer =  GLUtil::registerMesh(&cube);
    GLuint colorbuffer =  GLUtil::registerTexture(&cubeTexture);

    GLuint Texture = GLUtil::loadDDS("../src/main/resources/uvtemplate.DDS");
    GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

    Triangle triangle = Triangle();
    GLuint triangleVB =  GLUtil::registerMesh(&triangle);

    Camera camera = Camera();
    Input input = Input(false, false, false, false);

    FrameData frame = FrameData();

    do {

        GLUtil::nextFrame(frame);
        input = GLUtil::getInput(window);

        glm::mat4 View = GLUtil::getView(window, camera, input, frame.getTimeDelta());

        glm::mat4 Projection = glm::perspective(glm::radians(camera.initialFoV), 4.0f / 3.0f, 0.1f, 100.0f);
        glm::mat4 Model = glm::mat4(1.0f);
        glm::mat4 T = glm::mat4(1.0f);
        glm::mat4 MVP = Projection * View * T * Model;

        // Получить хэндл переменной в шейдере
        // Только один раз во время инициализации.
        GLuint MatrixID = glGetUniformLocation(programID, "MVP");

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glUniform1i(TextureID, 0);


        // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Draw nothing, see you in tutorial 2 !
        glUseProgram(programID);

        // Передать наши трансформации в текущий шейдер
        // Это делается в основном цикле, поскольку каждая модель будет иметь другую MVP-матрицу (как минимум часть M)
        glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

        // 1rst attribute buffer : vertices
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glVertexAttribPointer(
                0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void *) 0            // array buffer offset
        );
        // Второй буфер атрибутов - цвета
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glVertexAttribPointer(
                1,                                // Атрибут. Здесь необязательно указывать 1, но главное, чтобы это значение совпадало с layout в шейдере..
                3,                                // Размер
                GL_FLOAT,                         // Тип
                GL_FALSE,                         // Нормализован?
                0,                                // Шаг
                (void*)0                          // Смещение
        );


        // Draw the cube !
        glDrawArrays(GL_TRIANGLES, 0, 12*3); // 12*3 индексов начинающихся с 0. -> 12 треугольников -> 6 граней.

        glDisableVertexAttribArray(0);

        // Swap buffers
        glfwSwapBuffers(window);
        glfwPollEvents();

    } // Check if the ESC key was pressed or the window was closed
    while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
           glfwWindowShouldClose(window) == 0);

    // Close OpenGL window and terminate GLFW
    glfwTerminate();

    return 0;
}


