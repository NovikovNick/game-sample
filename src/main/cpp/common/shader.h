//
// Created by nicknovikov on 12/11/21.
//

#ifndef GAME_SHADER_H
#define GAME_SHADER_H

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "../model/model.cpp"

namespace GLUtil {

    GLuint LoadShaders(const char *vertex_file_path, const char *fragment_file_path);

    float animate(float from, float to, uint16_t frameSeqNumber, float velocity = 1);

    GLuint loadDDS(const char * imagepath);

    GLuint registerMesh(Mesh *mesh);

    GLuint registerTexture(Texture *texture);

    void nextFrame(FrameData &frameData);

    Input getInput(GLFWwindow *window);

    glm::mat4 getView(GLFWwindow *window,
                      Camera &camera,
                      const Input &input,
                      const float deltaTime);

}

#endif //GAME_SHADER_H

