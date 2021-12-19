// Include standard headers
#include <stdio.h>
#include <stdlib.h>

// Include GLEW
#include <GL/glew.h>

// #include<glad/glad.h>

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
    window = glfwCreateWindow(WIDTH, HEIGHT, "Captain Triangle conquers the universe", nullptr, nullptr);
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

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);

    // Create and compile our GLSL program from the shaders
    GLuint programID = GLUtil::LoadShaders("../src/main/glsl/SimpleVertexShader.vertexshader",
                                           "../src/main/glsl/SimpleFragmentShader.fragmentshader");

    // Vertices coordinates
    glm::vec4 corn0 = glm::vec4(0.5f,  0.5f, 0.0f, 1.0f);
    glm::vec4 corn1 = glm::vec4(0.5f, -0.5f, 0.0f,  1.0f);
    glm::vec4 corn2 = glm::vec4(-0.5f, -0.5f, 0.0f,  1.0f);
    glm::vec4 corn3 = glm::vec4(-0.5f,  0.5f, 0.0f,  1.0f);

    GLfloat vertices[] = {
            corn0[0], corn0[1], corn0[2],
            corn1[0], corn1[1], corn1[2],
            corn2[0], corn2[1], corn2[2],
            corn3[0], corn3[1], corn3[2]
    };

    GLuint indices[] = {
            0, 1, 3,   // fst triangle
            1, 2, 3    // snd triangle
    };

    // Create reference containers for the Vertex Array Object and the Vertex Buffer Object
    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Make the VAO the current Vertex Array Object by binding it
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    Camera camera = Camera();
    Input input = Input(false, false, false, false);
    FrameData frame = FrameData();
    do {

        // 1. calculate
        GLUtil::nextFrame(frame);
        input = GLUtil::getInput(window);

        glm::mat4 Projection = glm::perspective(glm::radians(camera.initialFoV), 4.0f / 3.0f, 0.1f, 100.0f);
        glm::mat4 Model = glm::mat4(1.0f);
        glm::mat4 View = GLUtil::getView(window, camera, input, frame.getTimeDelta());
        glm::mat4 MVP = Projection * View * Model;


        if (frame.frame > 100) {

            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

            /*corn0 = glm::vec4(GLUtil::animate(-1.0f, 1.0f, frame.frame), corn0[1], corn0[2], corn0[3]);
            corn1 = glm::vec4(corn1[0], GLUtil::animate(-1.0f, 1.0f, frame.frame), corn1[2], corn1[3]);
            corn2 = glm::vec4(GLUtil::animate(-1.0f, 1.0f, frame.frame), corn2[1], corn2[2], corn2[3]);


            GLfloat vertices2[] = {
                    corn0[0], corn0[1], corn0[2],
                    corn1[0], corn1[1], corn1[2],
                    corn2[0], corn2[1], corn2[2]
            };

            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            // Introduce the vertices into the VBO
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertices2), vertices2, GL_STREAM_DRAW);*/

        }
        // 2. render
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(programID);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));

    // Delete all the objects we've created
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(programID);
    // Delete window before ending the program
    glfwDestroyWindow(window);
    // Terminate GLFW before ending the program
    glfwTerminate();

    return 0;
}

