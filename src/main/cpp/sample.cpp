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

class Shape {
public:
    GLuint VAO, VBO, EBO;

    Shape() {
        GLfloat vertices[] = {
                0.5f,  0.5f, 0.5f,
                0.5f, -0.5f, 0.5f,
                -0.5f, -0.5f, 0.5f,
                -0.5f,  0.5f, 0.5f,
                0.5f,  0.5f, -0.5f,
                0.5f, -0.5f, -0.5f,
                -0.5f, -0.5f, -0.5f,
                -0.5f,  0.5f, -0.5f
        };

        GLuint indices[] = {
                0, 2, 3,   4, 6, 7,
                0, 1, 2,   4, 5, 6,

                0, 1, 5,   3, 2, 6,
                5, 4, 0,   6, 7, 3
        };

        // Create reference containers for the Vertex Array Object and the Vertex Buffer Object
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
    }

    virtual ~Shape() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }
};


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

    Shape shape1 = Shape();
    Shape shape2 = Shape();
    Shape shape3 = Shape();

    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


    Camera camera = Camera();
    Input input = Input(false, false, false, false);
    FrameData frame = FrameData();
    do {

        // 1. calculate
        GLUtil::nextFrame(frame);
        input = GLUtil::getInput(window);

        glm::mat4 View = GLUtil::getView(window, camera, input, frame.getTimeDelta());
        glm::mat4 Projection = glm::perspective(glm::radians(camera.initialFoV), 4.0f / 3.0f, 0.1f, 100.0f);
        glm::mat4 Model = glm::mat4(1.0f);
        glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(1, 0, 0));

        // 2. render
        glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        GLfloat timeValue = glfwGetTime();
        GLfloat greenValue = (sin(timeValue) / 2) + 0.5;
        GLint vertexColorLocation = glGetUniformLocation(programID, "customColor");
        GLint MVPLocation = glGetUniformLocation(programID, "MVP");

        {
            glm::mat4 MVP = Projection * View * Model;
            glUseProgram(programID);
            glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);
            glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &MVP[0][0]);

            glBindVertexArray(shape1.VAO);
            glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

        {
            glm::mat4 MVP = Projection * View * T * Model;
            glUseProgram(programID);
            glUniform4f(vertexColorLocation, greenValue, 0.0f,  0.0f, 1.0f);
            glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &MVP[0][0]);

            glBindVertexArray(shape2.VAO);
            glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

        {
            glm::mat4 MVP = Projection * View * glm::translate(Model, glm::vec3(-1, 0, 0));
            glUseProgram(programID);
            glUniform4f(vertexColorLocation, 0.0f,  0.0f, greenValue,  1.0f);
            glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &MVP[0][0]);

            glBindVertexArray(shape3.VAO);
            glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    } while (glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window));

    // Delete all the objects we've created
    glDeleteProgram(programID);
    // Delete window before ending the program
    glfwDestroyWindow(window);
    // Terminate GLFW before ending the program
    glfwTerminate();

    return 0;
}
