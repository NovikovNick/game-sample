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

GLFWwindow *window;

uint16_t frame = 0;

class Mesh {
public:
    virtual GLfloat *getVertexBufferData() = 0;
    virtual int size() = 0;
};

class Texture {
public:
    virtual GLfloat *getVertexBufferData() = 0;
    virtual int size() = 0;
};

class Cube : public Mesh {
public:
    ~Cube() {
        // delete[] vertex_buffer_data;
    }

    GLfloat *getVertexBufferData() override {
        return vertex_buffer_data;
    }

    int size() override{
        return sizeof(vertex_buffer_data);
    }

private:
    GLfloat vertex_buffer_data[36 * 3] = {
            -1.0f, -1.0f, -1.0f, // triangle 1 : start
            -1.0f, -1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f, // triangle 1 : end
            1.0f, 1.0f, -1.0f, // triangle 2 : start
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f, // triangle 2 : end
            1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,
            -1.0f, -1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, -1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, -1.0f,
            1.0f, -1.0f, -1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, -1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, -1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 1.0f
    };
};

class CubeTexture : public Texture {
public:
    ~CubeTexture() {
        // delete[] vertex_buffer_data;
    }

    GLfloat *getVertexBufferData() override {
        return g_color_buffer_data;
    }

    int size() override{
        return sizeof(g_color_buffer_data);
    }

private:
    GLfloat g_color_buffer_data[36 * 3] = {
            0.583f,  0.771f,  0.014f,
            0.609f,  0.115f,  0.436f,
            0.327f,  0.483f,  0.844f,
            0.822f,  0.569f,  0.201f,
            0.435f,  0.602f,  0.223f,
            0.310f,  0.747f,  0.185f,
            0.597f,  0.770f,  0.761f,
            0.559f,  0.436f,  0.730f,
            0.359f,  0.583f,  0.152f,
            0.483f,  0.596f,  0.789f,
            0.559f,  0.861f,  0.639f,
            0.195f,  0.548f,  0.859f,
            0.014f,  0.184f,  0.576f,
            0.771f,  0.328f,  0.970f,
            0.406f,  0.615f,  0.116f,
            0.676f,  0.977f,  0.133f,
            0.971f,  0.572f,  0.833f,
            0.140f,  0.616f,  0.489f,
            0.997f,  0.513f,  0.064f,
            0.945f,  0.719f,  0.592f,
            0.543f,  0.021f,  0.978f,
            0.279f,  0.317f,  0.505f,
            0.167f,  0.620f,  0.077f,
            0.347f,  0.857f,  0.137f,
            0.055f,  0.953f,  0.042f,
            0.714f,  0.505f,  0.345f,
            0.783f,  0.290f,  0.734f,
            0.722f,  0.645f,  0.174f,
            0.302f,  0.455f,  0.848f,
            0.225f,  0.587f,  0.040f,
            0.517f,  0.713f,  0.338f,
            0.053f,  0.959f,  0.120f,
            0.393f,  0.621f,  0.362f,
            0.673f,  0.211f,  0.457f,
            0.820f,  0.883f,  0.371f,
            0.982f,  0.099f,  0.879f
    };
};


class Triangle : public Mesh {
public:
    ~Triangle() {
        // delete[] vertex_buffer_data;
    }

    GLfloat *getVertexBufferData() override {
        return vertex_buffer_data;
    }

    int size() override{
        return sizeof(vertex_buffer_data);
    }

private:
    GLfloat vertex_buffer_data[3 * 3] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            0.0f, 1.0f, 0.0f
    };
};

class MeshService {
public:
    GLuint registerMesh(Mesh *mesh) {
        GLuint vertexbuffer;
        glGenBuffers(1, &vertexbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
        glBufferData(GL_ARRAY_BUFFER, mesh->size(), mesh->getVertexBufferData(), GL_STATIC_DRAW);
        return vertexbuffer;
    }

    GLuint registerTexture(Texture *texture) {
        GLuint colorbuffer;
        glGenBuffers(1, &colorbuffer);
        glBindBuffer(GL_ARRAY_BUFFER, colorbuffer);
        glBufferData(GL_ARRAY_BUFFER, texture->size(), texture->getVertexBufferData(), GL_STATIC_DRAW);
        return colorbuffer;
    }
};

GLuint loadDDS(const char *string);

int main(void) {
    // Initialise GLFW
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
    window = glfwCreateWindow(1024, 768, "Captain Triangle conquers the universe", NULL, NULL);
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
    glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

    GLuint VertexArrayID;
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);


    // Create and compile our GLSL program from the shaders
    /*GLuint programID = Graphic::LoadShaders("../src/main/glsl/SimpleVertexShader.vertexshader",
                                            "../src/main/glsl/SimpleFragmentShader.fragmentshader");*/
    GLuint programID = Graphic::LoadShaders("../src/main/glsl/TransformVertexShader.vertexshader",
                                            "../src/main/glsl/TextureFragmentShader.fragmentshader");




    // mesh
    Cube cube = Cube();
    CubeTexture cubeTexture = CubeTexture();

    MeshService service = MeshService();
    GLuint vertexbuffer = service.registerMesh(&cube);
    GLuint colorbuffer = service.registerTexture(&cubeTexture);

    GLuint Texture = loadDDS("../src/main/resources/uvtemplate.DDS");
    GLuint TextureID  = glGetUniformLocation(programID, "myTextureSampler");

    // Triangle triangle = Triangle();
    // GLuint triangleVB = service.registerMesh(&triangle);

    do {
        frame++;

        // Проекционная матрица : 45&deg; поле обзора, 4:3 соотношение сторон, диапазон : 0.1 юнит <-> 100 юнитов
        glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);

        // Или, для ортокамеры
        glm::mat4 View       = glm::lookAt(
                glm::vec3(4,3,3), // Камера находится в мировых координатах (4,3,3)
                glm::vec3(0,0,0), // И направлена в начало координат
                glm::vec3(0,1,0)  // "Голова" находится сверху
        );
        // Матрица модели : единичная матрица (Модель находится в начале координат)
        glm::mat4 Model = glm::mat4(1.0f);  // Индивидуально для каждой модели

        // glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(Graphic::animate(0, 1, frame, 0.5), 0, 0));
        glm::mat4 T = glm::mat4(1.0f);
        glm::mat4 R = glm::rotate(glm::mat4(1.0f), Graphic::animate(0, 6.3, frame, 0.1), glm::vec3(0, 1, 0));

        // Итоговая матрица ModelViewProjection, которая является результатом перемножения наших трех матриц
        glm::mat4 MVP = Projection * View * R * T * Model; // Помните, что умножение матрицы производиться в обратном порядке

        // Получить хэндл переменной в шейдере
        // Только один раз во время инициализации.
        GLuint MatrixID = glGetUniformLocation(programID, "MVP");

        // Bind our texture in Texture Unit 0
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Texture);
        // Set our "myTextureSampler" sampler to use Texture Unit 0
        glUniform1i(TextureID, 0);


        // Clear the screen. It's not mentioned before Tutorial 02, but it can cause flickering, so it's there nonetheless.
        glClear(GL_COLOR_BUFFER_BIT);

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


        // Draw the triangle !
        /*glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, triangleVB);
        glVertexAttribPointer(
                0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
                3,                  // size
                GL_FLOAT,           // type
                GL_FALSE,           // normalized?
                0,                  // stride
                (void *) 0            // array buffer offset
        );
        glDrawArrays(GL_TRIANGLES, 0, 3);*/

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


#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

GLuint loadDDS(const char * imagepath){

    unsigned char header[124];

    FILE *fp;

    /* try to open the file */
    fp = fopen(imagepath, "rb");
    if (fp == NULL){
        printf("%s could not be opened. Are you in the right directory ? Don't forget to read the FAQ !\n", imagepath); getchar();
        return 0;
    }

    /* verify the type of file */
    char filecode[4];
    fread(filecode, 1, 4, fp);
    if (strncmp(filecode, "DDS ", 4) != 0) {
        fclose(fp);
        return 0;
    }

    /* get the surface desc */
    fread(&header, 124, 1, fp);

    unsigned int height      = *(unsigned int*)&(header[8 ]);
    unsigned int width	     = *(unsigned int*)&(header[12]);
    unsigned int linearSize	 = *(unsigned int*)&(header[16]);
    unsigned int mipMapCount = *(unsigned int*)&(header[24]);
    unsigned int fourCC      = *(unsigned int*)&(header[80]);


    unsigned char * buffer;
    unsigned int bufsize;
    /* how big is it going to be including all mipmaps? */
    bufsize = mipMapCount > 1 ? linearSize * 2 : linearSize;
    buffer = (unsigned char*)malloc(bufsize * sizeof(unsigned char));
    fread(buffer, 1, bufsize, fp);
    /* close the file pointer */
    fclose(fp);

    unsigned int components  = (fourCC == FOURCC_DXT1) ? 3 : 4;
    unsigned int format;
    switch(fourCC)
    {
        case FOURCC_DXT1:
            format = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT;
            break;
        case FOURCC_DXT3:
            format = GL_COMPRESSED_RGBA_S3TC_DXT3_EXT;
            break;
        case FOURCC_DXT5:
            format = GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;
            break;
        default:
            free(buffer);
            return 0;
    }

    // Create one OpenGL texture
    GLuint textureID;
    glGenTextures(1, &textureID);

    // "Bind" the newly created texture : all future texture functions will modify this texture
    glBindTexture(GL_TEXTURE_2D, textureID);
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);

    unsigned int blockSize = (format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) ? 8 : 16;
    unsigned int offset = 0;

    /* load the mipmaps */
    for (unsigned int level = 0; level < mipMapCount && (width || height); ++level)
    {
        unsigned int size = ((width+3)/4)*((height+3)/4)*blockSize;
        glCompressedTexImage2D(GL_TEXTURE_2D, level, format, width, height,
                               0, size, buffer + offset);

        offset += size;
        width  /= 2;
        height /= 2;

        // Deal with Non-Power-Of-Two textures. This code is not included in the webpage to reduce clutter.
        if(width < 1) width = 1;
        if(height < 1) height = 1;

    }

    free(buffer);

    return textureID;


}


