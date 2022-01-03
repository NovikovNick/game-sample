#include <algorithm>
#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>

#include <graphic/GraphicProvider.h>
#include <graphic/opengl/OpenGLGraphicProvider.cpp>
#include "common/shader.h"

int main(void) {

    GraphicProvider *graphicProvider = new OpenGLGraphicProvider();
    graphicProvider->init();

    glm::vec3 position = glm::vec3(5, 5, 0);

    GameObject *obj = graphicProvider->register_objects();

    graphicProvider->start([&](const GraphicProvider *provider,
                               const Input input,
                               const FrameData frame,
                               const glm::mat4 View) {
        float d = 0.08;
        position = glm::vec3(
                position.x + (input.isAPressed ? -d : (input.isDPressed ? d : 0)),
                position.y + (input.isWPressed ? d : (input.isSPressed ? -d : 0)),
                0);
        position = glm::vec3(
                (position.x > 11 ? 11 : (position.x < 1 ? 1 : position.x)),
                (position.y > 8 ? 8 : (position.y < 1 ? 1 : position.y)),
                0);

        glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
        glm::mat4 Model = glm::mat4(1.0f);
        glm::mat4 T = glm::translate(glm::mat4(1.0f), glm::vec3(1, 0, 0));


        GLfloat timeValue = glfwGetTime();
        GLfloat color = (sin(timeValue) / 2) + 0.5;

        for (int x = 0; x < 13; ++x) {
            for (int y = 0; y < 10; ++y) {
                if (x == 0 || x == 12 || y == 0 || y == 9) {

                    glm::mat4 MVP = Projection * View * glm::translate(Model, glm::vec3(x, y, 0));
                    provider->render(obj, MVP);
                };
            }
        }
        glm::mat4 MVP = Projection * View * glm::translate(Model, position);
        provider->render(obj, MVP);
    });
    return 0;
}
