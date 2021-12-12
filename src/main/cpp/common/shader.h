//
// Created by nicknovikov on 12/11/21.
//

#ifndef GAME_SHADER_H
#define GAME_SHADER_H

#include <GL/glew.h>

namespace Graphic {

    GLuint LoadShaders(const char *vertex_file_path, const char *fragment_file_path);

    float animate(float from, float to, uint16_t frameSeqNumber, float velocity = 1);
}

#endif //GAME_SHADER_H

