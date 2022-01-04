#include <iostream>
#include <unordered_map>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <graphic/GraphicProvider.h>
#include <graphic/opengl/OpenGLGraphicProvider.cpp>
#include <model/GameObject.h>
#include <model/GameState.h>

GameState GenerateGameState(GraphicMetaData *pCubeMetaData);

inline float GetOffsetX(const Input &input, float delta) {
    return (input.isAPressed ? -delta : (input.isDPressed ? delta : 0));
}

inline float GetOffsetY(const Input &input, float delta) {
    return (input.isWPressed ? delta : (input.isSPressed ? -delta : 0));
}

inline float LimitBetween(float from, float to, float value) {
    return (value > to ? to : (value < from ? from : value));
}

int main(void) {

    GraphicProvider *graphicProvider = new OpenGLGraphicProvider();
    graphicProvider->Init();
    GraphicMetaData *pCubeMetaData = graphicProvider->GetCubeGraphicMetaData();

    GameState state = GenerateGameState(pCubeMetaData);

    graphicProvider->OnMainLoop([&](const GraphicProvider *provider,
                               const Input input,
                               const FrameData frame,
                               const glm::mat4 View) {

        // get enemies inputs
        Input enemiesInput = Input(0, 0, ~input.isWPressed,  ~input.isAPressed,  ~input.isSPressed,  ~input.isSPressed);

        // apply inputs
        state.player->position = glm::vec3(
                LimitBetween(state.leftBound + 1, state.rightBound - 1, state.player->position.x + GetOffsetX(input, state.player_speed)),
                LimitBetween(state.bottomBound + 1, state.topBound - 1, state.player->position.y + GetOffsetY(input, state.player_speed)),
                0);

        state.enemies[1]->position = glm::vec3(
                LimitBetween(state.leftBound + 1, state.rightBound - 1,state.enemies[1]->position.x + GetOffsetX(enemiesInput, state.player_speed)),
                LimitBetween(state.bottomBound + 1, state.topBound - 1, state.enemies[1]->position.y + GetOffsetY(enemiesInput, state.player_speed)),
                0);

        glm::mat4 Projection = glm::perspective(glm::radians(45.0f), 4.0f / 3.0f, 0.1f, 100.0f);
        glm::mat4 Model = glm::mat4(1.0f);

        // render
        for (const auto &it : state.all) {
            provider->Render(it->graphic_metadata, Projection * View * glm::translate(Model, it->position));
        }
    });
    return 0;
}

GameState GenerateGameState(GraphicMetaData *pCubeMetaData) {
    GameState state;

    // generate state
    int index = 0;
    for (int32_t x = state.leftBound; x <= state.rightBound; ++x) {
        for (int32_t y = state.bottomBound; y <= state.topBound; ++y) {

            if (x == state.leftBound || x == state.rightBound || y == state.topBound || y == state.bottomBound) {
                GameObject *pWall = new GameObject();
                pWall->position = glm::vec3(x, y, 0);
                pWall->graphic_metadata = pCubeMetaData;
                state.all.push_back(pWall);
                index++;
            };
        }
    }
    {
        state.player = new GameObject();
        state.player->position = glm::vec3(5, 5, 0);
        state.player->graphic_metadata = pCubeMetaData;
        state.all.push_back(state.player);
    }

    {
        int playerId = 1;
        GameObject *enemy = new GameObject();
        enemy->position = glm::vec3(8, 6, 0);
        enemy->graphic_metadata = pCubeMetaData;
        state.enemies[playerId] = enemy;
        state.all.push_back(enemy);
    }
    return state;
}
