#ifndef GAME_CAMERA_MODEL_H
#define GAME_CAMERA_MODEL_H

#include <GL/glew.h>
#include <glm/vec3.hpp>

struct Camera {
public:
    float mouseSpeed = 0.01;
    float initialFoV = 45.0f;
    float horizontalAngle = -9.42;
    float verticalAngle = 0.00999486;
    glm::vec3 position = glm::vec3(6.05218, 4.39045, 11.3271);
};
#endif //GAME_CAMERA_MODEL_H
