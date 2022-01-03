#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <graphic/GraphicProvider.h>
#include <graphic/opengl/OpenGLGraphicProvider.cpp>
#include <model/GameObject.h>

inline float getOffsetX(const Input &input, float delta) {
    return (input.isAPressed ? -delta : (input.isDPressed ? delta : 0));
}

inline float getOffsetY(const Input &input, float delta) {
    return (input.isWPressed ? delta : (input.isSPressed ? -delta : 0));
}

inline float limitBetween(float from, float to, float value) {
    return (value > to ? to : (value < from ? from : value));
}

int main(void) {

    GraphicProvider *graphicProvider = new OpenGLGraphicProvider();
    graphicProvider->init();

    GraphicMetaData *pCubeMetaData = graphicProvider->register_objects();

    GameObject walls[42];
    int index = 0;
    for (int x = 0; x < 13; ++x) {
        for (int y = 0; y < 10; ++y) {
            if (x == 0 || x == 12 || y == 0 || y == 9) {
                walls[index] = GameObject();
                walls[index].position = glm::vec3(x, y, 0);
                walls[index].graphic_metadata = pCubeMetaData;
                index++;
            };
        }
    }
    GameObject player;
    player = GameObject();
    player.position = glm::vec3(5, 5, 0);
    player.graphic_metadata = pCubeMetaData;

    float delta = 0.08;

    graphicProvider->start([&](const GraphicProvider *provider,
                               const Input input,
                               const FrameData frame,
                               const glm::mat4 View) {

        player.position = glm::vec3(
                limitBetween(1, 11, player.position.x + getOffsetX(input, delta)),
                limitBetween(1, 8, player.position.y + getOffsetY(input, delta)),
                0);

        glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
        glm::mat4 Model = glm::mat4(1.0f);

        for (int i = 0; i < 42; ++i) {
            provider->render(walls[i].graphic_metadata, Projection * View * glm::translate(Model, walls[i].position));
        }
        provider->render(player.graphic_metadata, Projection * View * glm::translate(Model, player.position));
    });
    return 0;
}
