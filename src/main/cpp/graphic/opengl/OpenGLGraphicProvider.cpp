#include "graphic/GraphicProvider.h"

#include <stdio.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string.h>
#include <sstream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include <model/Camera.h>


class OpenGLGraphicMetaData : public GraphicMetaData {
public:
    GLuint VAO, VBO, texture, shaderID;
};

class OpenGLGraphicProvider : public GraphicProvider {
public:
    void Init() override {
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

    GraphicMetaData *GetCubeGraphicMetaData() override {


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
        shaderID = LoadShaders("../src/main/glsl/SimpleVertexShader.vertexshader",
                               "../src/main/glsl/SimpleFragmentShader.fragmentshader");


        OpenGLGraphicMetaData *pObject = new OpenGLGraphicMetaData();
        pObject->VAO = VAO;
        pObject->VBO = VBO;
        pObject->texture = texture;
        pObject->shaderID = shaderID;
        return pObject;
    }

    /*void DestroyObjects(GraphicMetaData *gameObject) const override {

        OpenGLGraphicMetaData *it = (OpenGLGraphicMetaData *) gameObject;

        glDeleteVertexArrays(1, &it->VAO);
        glDeleteBuffers(1, &it->VBO);
        glDeleteProgram(it->shaderID);

        delete gameObject;
    }*/

    void Render(GraphicMetaData *gameObject, glm::mat4 MVP) const override {

        OpenGLGraphicMetaData *it = (OpenGLGraphicMetaData *) gameObject;

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

    void OnMainLoop(std::function<void(
            const GraphicProvider *provider,
            const Input input,
            const FrameData frame,
            const glm::mat4 View)> f) override {


        Camera camera = Camera();

        Input dummy = Input(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2,false, false, false, false);
        Input input = dummy;
        FrameData frame = FrameData();

        do {
            nextFrame(frame);

            input = getInput();
            glfwSetCursorPos(window_, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2);

            glm::mat4 View = getView(camera, input, frame.getTimeDelta());

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

private:
    int SCREEN_WIDTH = 1024;
    int SCREEN_HEIGHT = 768;
    GLFWwindow *window_ = NULL;

    Input getInput() {

        double xpos, ypos;
        glfwGetCursorPos(window_, &xpos, &ypos);

        return Input(
                xpos, ypos,
                glfwGetKey(window_, GLFW_KEY_W) == GLFW_PRESS,
                glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS,
                glfwGetKey(window_, GLFW_KEY_S) == GLFW_PRESS,
                glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS
        );
    }

    void nextFrame(FrameData &frameData) {

        frameData.frame = frameData.frame + 1;
        frameData.t0 = frameData.t1;
        frameData.t1 = glfwGetTime();
    }

    glm::mat4 getView(Camera &camera,
                      const Input &input,
                      const float deltaTime) {

        camera.horizontalAngle += camera.mouseSpeed * float(SCREEN_WIDTH / 2 - input.xPos);
        camera.verticalAngle += camera.mouseSpeed * float(SCREEN_HEIGHT / 2 - input.yPos);

        // Direction : Spherical coordinates to Cartesian coordinates conversion
        glm::vec3 direction(
                cos(camera.verticalAngle) * sin(camera.horizontalAngle),
                sin(camera.verticalAngle),
                cos(camera.verticalAngle) * cos(camera.horizontalAngle)
        );

        // Right vector
        glm::vec3 right = glm::vec3(
                sin(camera.horizontalAngle - 3.14f / 2.0f),
                0,
                cos(camera.horizontalAngle - 3.14f / 2.0f)
        );

        // Up vector
        glm::vec3 up = glm::cross(right, direction);


        /*std::cout   << "------------------Camera----------------" << std::endl
                    << "float horizontalAngle = " << camera.horizontalAngle  << ";\n"
                    << "float verticalAngle = " << camera.verticalAngle  << ";\n"
                    << "glm::vec3 position = glm::vec3(" << camera.position[0] << ", " << camera.position[1] << ", " << camera.position[2]  << ");\n";*/


        if (input.isWPressed) camera.position += direction * deltaTime * camera.mouseSpeed * 100.0f;
        if (input.isSPressed) camera.position -= direction * deltaTime * camera.mouseSpeed * 100.0f;
        if (input.isAPressed) camera.position -= right * deltaTime * camera.mouseSpeed * 100.0f;
        if (input.isDPressed) camera.position += right * deltaTime * camera.mouseSpeed * 100.0f;


        // Или, для ортокамеры
        glm::mat4 View = glm::lookAt(
                camera.position,           // Camera is here
                camera.position + direction, // and looks here : at the same position, plus "direction"
                up                  // Head is up (set to 0,-1,0 to look upside-down)
        );
        return View;
    }

    GLuint LoadShaders(const char *vertex_file_path, const char *fragment_file_path) {

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
};
