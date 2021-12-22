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
#include <SOIL.h>
#include "common/shader.h"
#include "common/Animation.h"

int WIDTH = 1024;
int HEIGHT = 768;

GLFWwindow *window;

class Shape {
public:
    GLuint VAO, VBO;

    Shape() {
        GLfloat vertices[] = {
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

                -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

                0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
                0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
                0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
                -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
                -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
                0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
                0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
                -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
                -0.5f, 0.5f, -0.5f, 0.0f, 1.0f
        };

        // Create reference containers for the Vertex Array Object and the Vertex Buffer Object
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);


        // Make the VAO the current Vertex Array Object by binding it
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) 0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid *) (3 * sizeof(GLfloat)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    virtual ~Shape() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
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
    glEnable(GL_DEPTH_TEST);


    // Load and create a texture
    GLuint texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D,
                  texture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object
    // Set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
                    GL_REPEAT);    // Set texture wrapping to GL_REPEAT (usually basic wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // Load image, create texture and generate mipmaps
    int width, height;
    unsigned char *image = SOIL_load_image("../src/main/resources/container.jpg", &width, &height, 0, SOIL_LOAD_RGB);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);
    SOIL_free_image_data(image);
    glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        GLfloat timeValue = glfwGetTime();
        GLfloat color = (sin(timeValue) / 2) + 0.5;
        GLint vertexColorLocation = glGetUniformLocation(programID, "customColor");
        GLint MVPLocation = glGetUniformLocation(programID, "MVP");

        for (int x = 0; x < 13; ++x) {
            for (int y = 0; y < 2; ++y) {
                glm::mat4 MVP = Projection * View * glm::translate(Model, glm::vec3(x, y, 0));
                glUseProgram(programID);
                glUniform4f(vertexColorLocation, 1.0f - color, 1.0f, 1.0f, 1.0f);
                glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &MVP[0][0]);

                glBindTexture(GL_TEXTURE_2D, texture);
                glUniform1i(glGetUniformLocation(programID, "ourTexture"), 0);

                glBindVertexArray(shape1.VAO);
                glDrawArrays(GL_TRIANGLES, 0, 36);
                glBindVertexArray(0);
            }
        }

         {

            glm::mat4 MVP = Projection * View * glm::translate(Model, glm::vec3(Animation::animate(1.0f, 11.0f, frame.frame, 0.5), 4, 0));
            glUseProgram(programID);
            glUniform4f(vertexColorLocation, 1.0f - color, 1.0f, 1.0f, 1.0f);
            glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &MVP[0][0]);

            glBindTexture(GL_TEXTURE_2D, texture);
            glUniform1i(glGetUniformLocation(programID, "ourTexture"), 0);

            glBindVertexArray(shape1.VAO);
            glDrawArrays(GL_TRIANGLES, 0, 36);
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
