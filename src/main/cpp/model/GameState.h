#ifndef GAME_GAME_STATE_H
#define GAME_GAME_STATE_H

#include <vector>
#include <unordered_map>

#include <glm/vec3.hpp>

struct GameState {
public:
    const int32_t leftBound = 0;
    const int32_t rightBound = 12;
    const int32_t bottomBound = 0;
    const int32_t topBound = 9;

    const float player_speed = 0.08;

    std::vector<GameObject*> all;
    std::unordered_map<int32_t, GameObject*> enemies;
    GameObject* player;
};
#endif //GAME_GAME_STATE_H
