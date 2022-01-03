#ifndef GAME_GAMEOBJECT_H
#define GAME_GAMEOBJECT_H


#include <glm/vec3.hpp>

struct GameObject {

    glm::vec3 position;
    GraphicMetaData* graphic_metadata;
};
#endif //GAME_GAMEOBJECT_H
