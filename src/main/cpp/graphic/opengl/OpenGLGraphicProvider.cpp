#include "graphic/GraphicProvider.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <cstring>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <SOIL.h>

#include "common/shader.h"


class OpenGLGameObject : public GameObject {
public:
    GLuint VAO, VBO, texture, shaderID;
};

class OpenGLGraphicProvider : public GraphicProvider {

    int SCREEN_WIDTH = 1024;
    int SCREEN_HEIGHT = 768;

    GLFWwindow *window_ = NULL;

    void init() override {
        if (!glfwInit()) {
            fprintf(stderr, "Failed to initialize GLFW\n");
            getchar();
            // return -1;
        }

        glfwWindowHint(GLFW_SAMPLES, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        // Open a window and create its OpenGL context
        window_ = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Captain Triangle conquers the universe", nullptr,
                                   nullptr);
        if (window_ == NULL) {
            fprintf(stderr,
                    "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
            getchar();
            glfwTerminate();
            // return -1;
        }
        glfwMakeContextCurrent(window_);

        glewExperimental = true; // Needed for core profile
        // Initialize GLEW
        if (glewInit() != GLEW_OK) {
            fprintf(stderr, "Failed to initialize GLEW\n");
            getchar();
            glfwTerminate();
            // return -1;
        }

        glfwSetInputMode(window_, GLFW_STICKY_KEYS, GL_TRUE);
        glfwSetCursorPos(window_, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);


        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glEnable(GL_DEPTH_TEST);

    }

    GameObject *register_objects() const override {


        //this->onFrameRender();


        GLuint VAO, VBO, texture, shaderID;

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

        // Load and create a texture
        // GLuint texture;
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
        unsigned char *image = SOIL_load_image("../src/main/resources/container.jpg", &width, &height, 0,
                                               SOIL_LOAD_RGB);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
        glGenerateMipmap(GL_TEXTURE_2D);
        SOIL_free_image_data(image);
        glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.


        // Create and compile our GLSL program from the shaders
        shaderID = GLUtil::LoadShaders("../src/main/glsl/SimpleVertexShader.vertexshader",
                                       "../src/main/glsl/SimpleFragmentShader.fragmentshader");


        OpenGLGameObject *pObject = new OpenGLGameObject();
        pObject->VAO = VAO;
        pObject->VBO = VBO;
        pObject->texture = texture;
        pObject->shaderID = shaderID;
        return pObject;
    }

    void destroy_objects(GameObject *gameObject) const override {

        OpenGLGameObject *it = (OpenGLGameObject *) gameObject;

        glDeleteVertexArrays(1, &it->VAO);
        glDeleteBuffers(1, &it->VBO);
        glDeleteProgram(it->shaderID);

        delete gameObject;
    }

    void render(GameObject *gameObject, glm::mat4 MVP) const override {

        OpenGLGameObject *it = (OpenGLGameObject *) gameObject;

        GLint vertexColorLocation = glGetUniformLocation(it->shaderID, "customColor");
        GLint MVPLocation = glGetUniformLocation(it->shaderID, "MVP");

        glUseProgram(it->shaderID);
        glUniform4f(vertexColorLocation, 1.0f, 1.0f, 1.0f, 1.0f);
        glUniformMatrix4fv(MVPLocation, 1, GL_FALSE, &MVP[0][0]);

        glBindTexture(GL_TEXTURE_2D, it->texture);
        glUniform1i(glGetUniformLocation(it->shaderID, "ourTexture"), 0);

        glBindVertexArray(it->VAO);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }

    void start(std::function<void(
            const GraphicProvider *provider,
            const Input input,
            const FrameData frame,
            const glm::mat4 View)> f) const override {


        Camera camera = Camera();

        Input input = Input(false, false, false, false);
        FrameData frame = FrameData();

        do {
            GLUtil::nextFrame(frame);
            input = GLUtil::getInput(window_);
            glm::mat4 View = GLUtil::getView(window_, camera, Input(false, false, false, false), frame.getTimeDelta());


            glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            f(this, input, frame, View);

            glfwSwapBuffers(window_);
            glfwPollEvents();

        } while (glfwGetKey(window_, GLFW_KEY_ESCAPE) != GLFW_PRESS && !glfwWindowShouldClose(window_));

        // Delete all the objects we've created
        // graphicProvider->destroy_objects(obj);

        // Delete window before ending the program
        glfwDestroyWindow(window_);
        // Terminate GLFW before ending the program
        glfwTerminate();
    }
};
